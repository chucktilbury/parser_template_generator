/*
 * Command line implementation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cmdline.h"
#include "alloc.h"
#include "strg_list.h"
#include "ptr_list.h"
#include "strgs.h"

// This is for options only. The values are stored in the config data
// structure. It's only used when parsing the command line.
typedef struct _cmdline_entry_t_ {
    int short_opt;
    string_t* long_opt;
    string_t* name;
    string_t* help;
    string_list_t* value;
    cmdline_callback_t cb;
    cmdline_type_t type;
} cmdline_entry_t;

typedef struct _cmdline_t_ {
    string_t* preamble;
    string_t* vers;
    string_t* name;
    string_t* pname;
    int files;
    ptr_list_t* args;
} cmdline_t;

#define EXPECTED(s)                                                                \
    do {                                                                           \
        fprintf(stderr, "CMDLINE: Expected %s but got '%c'\n\n", (s), get_char()); \
    } while(0)

#define ERROR(s, ...)                                  \
    do {                                               \
        fprintf(stderr, "CMDLINE: ");                  \
        fprintf(stderr, s __VA_OPT__(, ) __VA_ARGS__); \
        fprintf(stderr, "\n\n");                       \
    } while(0)


/*********************************************
 * Private functions
 */
static const char** cmds = NULL;
static int cmds_idx = 0;
static int max_cmds_idx = 0;
static cmdline_t* cmdline = NULL;


static void show_cmdline_vers(void) {

    printf("%s: v%s\n", raw_string(cmdline->name), raw_string(cmdline->vers));
}

static void show_cmdline_help(void) {

    cmdline_entry_t* ptr;
    int mark = 0;
    char tmp[128];

    show_cmdline_vers();
    printf("%s\n\n", raw_string(cmdline->preamble));
    printf("Use: %s [options] %s\n\n", raw_string(cmdline->pname),
           (cmdline->files > 1)          ? "[list of files]" :
                   (cmdline->files == 1) ? "[file name]" :
                                           "");

    memset(tmp, 0, sizeof(tmp));
    memset(tmp, '-', 80);
    printf("%s\n", tmp);

    while(NULL != (ptr = iterate_ptr_list(cmdline->args, &mark))) {
        if(ptr->short_opt != 0 || ptr->long_opt != NULL) {
            // this is an actual option.
            if(ptr->short_opt != 0)
                printf("  -%c ", ptr->short_opt);
            else
                printf("     ");

            if(ptr->long_opt != NULL)
                printf("--%-10s [", raw_string(ptr->long_opt));
            else {
                printf("             [");
            }

            memset(tmp, 0, sizeof(tmp));
            if(ptr->type & CMD_LIST)
                strcpy(tmp, "list of ");
            if(ptr->type & CMD_BOOL)
                strcat(tmp, "bool] ");
            else if(ptr->type & CMD_STR)
                strcat(tmp, "strg] ");
            else if(ptr->type & CMD_NUM)
                strcat(tmp, "num]  ");
            else
                strcat(tmp, "sw]   ");
            printf("%s", tmp);

            if(ptr->help != NULL)
                printf("%s %s\n", raw_string(ptr->help), ptr->type & CMD_REQD ? "(reqd)" : "");
        }
        else if(ptr->type & CMD_DIV) {
            // this is a divider
            memset(tmp, 0, sizeof(tmp));
            memset(tmp, '-', 80);
            printf("%s\n", tmp);
        }
        else {
            // it's a file list
            if(ptr->help != NULL)
                printf("  %s %s\n", raw_string(ptr->help), ptr->type & CMD_REQD ? "(reqd)" : "");
        }
    }

    memset(tmp, 0, sizeof(tmp));
    memset(tmp, '-', 80);
    printf("%s\n\n", tmp);
}

static void init_cmd(int argc, char** argv) {

    cmds_idx = 0;
    max_cmds_idx = argc;
    cmds = (const char**)argv;
}

static const char* get_cmd(void) {

    if(cmds_idx >= max_cmds_idx)
        return NULL;
    else
        return cmds[cmds_idx];
}

static const char* consume_cmd(void) {

    if(cmds_idx < max_cmds_idx)
        cmds_idx++;

    return get_cmd();
}

/*
 * Read the list that was created with add_cmdline() and return the option if
 * it exists.
 */
static cmdline_entry_t* get_short_opt(int ch) {

    cmdline_entry_t* ptr;
    int mark = 0;

    while(NULL != (ptr = iterate_ptr_list(cmdline->args, &mark))) {
        if(ptr->short_opt == ch)
            break;
    }

    return ptr;
}

/*
 * Read the list that was created with add_cmdline() and return the option if
 * it exists.
 */
static cmdline_entry_t* get_long_opt(const char* str) {

    cmdline_entry_t* ptr;
    int mark = 0;

    while(NULL != (ptr = iterate_ptr_list(cmdline->args, &mark))) {
        if(ptr->long_opt != NULL && !comp_string_str(ptr->long_opt, str))
            break;
    }

    return ptr;
}

/*
 * Search the command options for the given name and return the option if it
 * exists. Otherwise return NULL.
 */
static cmdline_entry_t* get_opt_by_name(const char* str) {

    cmdline_entry_t* ptr;
    int mark = 0;

    while(NULL != (ptr = iterate_ptr_list(cmdline->args, &mark))) {
        if(ptr->name != NULL && !comp_string_str(ptr->name, str))
            break;
    }

    return ptr;
}

const char* parm_type_to_str(cmdline_type_t type) {

    return ((type & CMD_STR) && !(type & (CMD_NUM | CMD_BOOL)))  ? "STR " :
            ((type & CMD_NUM) && !(type & (CMD_STR | CMD_BOOL))) ? "NUM " :
            ((type & CMD_BOOL) && !(type & (CMD_NUM | CMD_STR))) ? "BOOL " :
                                                                   "";
}

/*********************************************
 * APIs used by this software.
 */
void destroy_cmdline(void) {

    if(cmdline != NULL) {
        cmdline_entry_t* ptr;
        int mark = 0;

        if(cmdline->preamble != NULL)
            destroy_string(cmdline->preamble);
        if(cmdline->vers != NULL)
            destroy_string(cmdline->vers);
        if(cmdline->name != NULL)
            destroy_string(cmdline->name);
        if(cmdline->pname != NULL)
            destroy_string(cmdline->pname);

        while(NULL != (ptr = iterate_ptr_list(cmdline->args, &mark))) {
            if(ptr->long_opt != NULL)
                destroy_string(ptr->long_opt);
            if(ptr->name != NULL)
                destroy_string(ptr->name);
            if(ptr->help != NULL)
                destroy_string(ptr->help);

            if(ptr->value != NULL)
                destroy_string_list(ptr->value);
            _FREE(ptr);
        }

        _FREE(cmdline);
    }
}

/*
 * Add a parsed command line value to the value database. If the option
 * already exists and it's a list the add it to the list. If it's not a
 * list and it already exists, then flag an error.
 */
static void add_cmdline_arg(cmdline_entry_t* item, const char* str) {

    // printf("adding: %s = %s\n", raw_string(item->name), str);

    if((item->type & CMD_LIST) && (item->value != NULL)) {
        append_string_list(item->value, create_string(str));
    }
    else if(item->value != NULL) {
        int mark = 0;
        string_t* s = iterate_string_list(item->value, &mark); // get the first item.
        clear_string(s);
        append_string(s, str);
    }
    else {
        item->value = create_string_list();
        append_string_list(item->value, create_string(str));
    }
}

/*
 * Parse a short option. Examples:
 * A short option can require an argument or not. If a arg is specified, then
 * whatever follows it is considered to be that argument.
 *
 * No args required.
 * -abcd
 * is equivalent to -a -b -c -d
 *
 * "-a" requires args
 * -abc something
 * is equivalent to -a bc something
 *
 * "-a" does not require args, but "-c" does, then
 * -abc something
 * is equivalent to -a -b -c something
 *
 * If "-a" requires and arg then
 * -a=10
 * is equivalent to -a 10
 * Else if "-a" does not require an arg, then it's a syntax error.
 *
 * If "-a" requires an arg then
 * -a10
 * is equivalent to -a=10
 * Else "-1" is an invalid option.
 *
 * If "-a" requires an arg then
 * -a-10
 * is equivalent to -a=-10
 * Else "-1" is an invalid option.
 *
 */
static void parse_short_option(const char* str) {

    // printf("sstr: %s\n", str);

    cmdline_entry_t* item;
    int idx = 0;
    int finished = 0;

    while(!finished) {

        item = get_short_opt(str[idx]);
        if(item != NULL) {
            if(item->cb != NULL) {
                (*item->cb)();
                return;
            }

            // printf("setting 'seen' on %s\n", item->name);
            item->type |= CMD_SEEN;
            if(item->type & CMD_ARGS) {
                if(str[idx + 1] == '=' && str[idx + 2] != '\0') {
                    printf("here\n");
                    add_cmdline_arg(item, &str[idx + 2]);
                    consume_cmd();
                    return;
                }
                else if(str[idx + 1] != '\0') {
                    add_cmdline_arg(item, &str[idx + 1]);
                    consume_cmd();
                    return;
                }
                else {
                    str = consume_cmd();
                    if(str != NULL) {
                        add_cmdline_arg(item, str);
                        consume_cmd();
                        return;
                    }
                    else {
                        ERROR("short command option \"-%c\" requires argument", str[idx]);
                        show_cmdline_help();
                        exit(1);
                    }
                }
            }
        }
        else {
            ERROR("unknown short command option: \"-%c\"", str[idx]);
            show_cmdline_help();
            exit(1);
        }

        idx++;
        if(str[idx] == '\0')
            finished++;
    }

    consume_cmd();
}

/*
 * Parse a long option.
 *
 * A long option can require an argument or not and it follows the same
 * general rules as a short option. but the difference is that a long option
 * cannot be combined as a short option.
 *
 * if "--abc" does not require an arg then
 * -abc something
 * is a separate set of options, and
 * --abc=something
 * is a syntax error.
 *
 * If "--abc" does require an arg, then
 * --abc=something
 * and
 * --abc something
 * are equivalent.
 *
 */
static void parse_long_option(const char* str) {

    // printf("lstr: %s\n", str);

    char* tpt = _COPY_STRING(str);
    char* arg = NULL;

    if(NULL != (arg = strchr(tpt, '='))) {
        *arg = '\0';
        arg++;
    }

    cmdline_entry_t* item = get_long_opt(tpt);

    if(item != NULL) {
        if(item->cb != NULL) {
            _FREE(tpt);
            (*item->cb)();
            return;
        }

        // printf("setting 'seen' on %s\n", item->name);
        item->type |= CMD_SEEN;
        if(item->type & CMD_ARGS) {
            if(arg == NULL) {
                const char* str = consume_cmd();
                if(str != NULL) {
                    add_cmdline_arg(item, str);
                    consume_cmd();
                    _FREE(tpt);
                    return;
                }
                else {
                    ERROR("long command option \"--%s\" requires argument", tpt);
                    show_cmdline_help();
                    exit(1);
                }
            }
            else if(arg[0] != '\0') {
                add_cmdline_arg(item, arg);
                consume_cmd();
                _FREE(tpt);
                return;
            }
            else {
                ERROR("expected an argument after \"--%s\"", tpt);
                show_cmdline_help();
                exit(1);
            }
        }
    }
    else {
        ERROR("unknown long command option: \"--%s\"", tpt);
        show_cmdline_help();
        exit(1);
    }

    _FREE(tpt);
    consume_cmd();
}

/*
 * Get a command option by name. If the option does not exist then return NULL.
 */
const string_t* get_cmd_opt(const char* name) {

    cmdline_entry_t* opt = get_opt_by_name(name);
    int mark = 0;

    if(opt != NULL)
        return iterate_string_list(opt->value, &mark);
    else
        return NULL;
}

/*
 * Iterate the option as a list.
 */
const string_t* iterate_cmd_opt(const char* name, int* mark) {

    cmdline_entry_t* opt = get_opt_by_name(name);

    if(opt != NULL)
        return iterate_string_list(opt->value, mark);
    else
        return NULL;
}

/*
 * When this is entered, a command line item has been encountered that does not
 * start with a '-'. If there is a option that allows a raw list then the
 * string is stored as it is found. otherwise, it is an "unknown option"
 * error.
 *
 * Note that an exception is made when the raw option starts with a '-' but
 * there are spaces in it. That means that it had quotes around it. and so it
 * is a single entity.
 */
static void parse_list_item(const char* str) {

    cmdline_entry_t* item;
    int mark = 0;

    while(NULL != (item = iterate_ptr_list(cmdline->args, &mark))) {
        if(item->type & CMD_ANON)
            break;
    }

    if(item != NULL) {
        add_cmdline_arg(item, str);
        item->type |= CMD_SEEN;
    }
    else {
        ERROR("unknown option: \"%s\"", str);
        show_cmdline_help();
        exit(1);
    }
    consume_cmd();
}

/*
 * Parse the command line.
 *
 * Possible bug:
 * Bash strips quotes around items. If the item is in quotes and starts with a
 * dash (-) then it is mistaken for a command option. This could be a problem
 * if it's needed to specify command parameters for a child program. One
 * solution may be to have the parser expect to ignore dashes in the value. But
 * that leads to other possible issues.
 *
 * Question:
 * Does winders also strip quotes? Are there issues with not using a slash (/)
 * to intro command options?
 */
void parse_cmdline(int argc, char** argv) {

    init_cmd(argc, argv);

    cmdline->pname = create_string(argv[0]);
    const char* ptr = consume_cmd(); // discard the first element.

    while(1) {
        if(NULL != (ptr = get_cmd())) {
            if(ptr[0] == '-') {
                // parse the item with the leading '-' removed
                if(ptr[1] == '-')
                    parse_long_option(&ptr[2]);
                else
                    parse_short_option(&ptr[1]);
            }
            else {
                parse_list_item(ptr);
            }
        }
        else
            break;
    }

    // process the required command options after this.
    cmdline_entry_t* opt;
    int mark = 0;

    while(NULL != (opt = iterate_ptr_list(cmdline->args, &mark))) {
        if(opt->type & CMD_REQD && !(opt->type & CMD_SEEN)) {
            if(opt->long_opt != NULL)
                ERROR("required command option not found: \"--%s\"", raw_string(opt->long_opt));
            else if(opt->short_opt != 0)
                ERROR("required command option not found: \"-%c\"", opt->short_opt);
            else if(opt->name != NULL)
                ERROR("required command option not found: \"%s\"", raw_string(opt->name));

            show_cmdline_help();
            exit(1);
        }
    }
}


/*********************************************
 * Callbacks used by the built-in command options.
 */

/*********************************************
 * APIs called by the user's application
 */

void init_cmdline(const char* name, const char* preamble, const char* vers) {

    cmdline = _ALLOC_TYPE(cmdline_t);
    cmdline->vers = create_string(vers);
    cmdline->preamble = create_string(preamble);
    cmdline->name = create_string(name);
    cmdline->args = create_ptr_list();
    cmdline->files = 0;
}

void add_cmdline(int short_opt,
                 const char* long_opt,
                 const char* name,
                 const char* help,
                 const char* def_val,
                 cmdline_callback_t cb,
                 cmdline_type_t type) {

    cmdline_entry_t* ptr = _ALLOC_TYPE(cmdline_entry_t);

    if(!(type & CMD_DIV) && short_opt == 0 && long_opt == NULL) {
        if(type & CMD_LIST)
            cmdline->files = 2;
        else
            cmdline->files = 1;
    }

    if(long_opt != NULL)
        ptr->long_opt = create_string(long_opt);

    if(name != NULL)
        ptr->name = create_string(name);

    if(help != NULL)
        ptr->help = create_string(help);

    if(def_val != NULL) {
        ptr->value = create_string_list();
        append_string_list(ptr->value, create_string(def_val));
    }

    ptr->short_opt = short_opt;
    ptr->cb = cb;
    ptr->type = type;

    if(type & CMD_LIST)
        ptr->type |= CMD_ARGS;

    append_ptr_list(cmdline->args, ptr);
}

void cb_cmdline_help(void) {

    show_cmdline_help();
    exit(1);
}

void cb_cmdline_vers(void) {

    show_cmdline_vers();
    exit(1);
}
