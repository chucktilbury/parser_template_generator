HIDE = @
TARGET = pgen
DEPS = $(TARGET).deps
OBJS = 	parser.o \
		scanner.o \
		main.o \
		alloc.o \
		ptrlist.o \
		strbuf.o \
		strlist.o \
		tokens.o \
		ast.o \
		template.o \
		hash.o \
		render.o \
		lists.o \
		term_list.o \
		nterm_list.o \
		trace.o

HEADERS = trace.h \
		errors.h \
		common.h

#DEBUG = -DUSE_TRACE -g
DEBUG = -g
CARGS = -Wall -Wextra $(DEBUG)
CC = clang

%.o:%.c
	@ echo "compile: $@"
	$(HIDE) $(CC) $(CARGS) -c -o $@ $<

$(TARGET): $(OBJS) $(DEPS)
	@ echo "link: $(TARGET)"
	$(HIDE) $(CC) -g -o $(TARGET) $(OBJS)

template.c template.h: template.l
	flex template.l

scanner.c scanner.h: scanner.l
	flex scanner.l

parser.c parser.h: parser.y
	bison parser.y

$(DEPS): $(OBJS:.o=.c)
	@ echo "depends"
	$(HIDE) $(CC) -MM $^ > $(DEPS)

include $(DEPS)

.PHONY: clean
clean:
	@ echo "clean"
	$(HIDE) $(RM) $(TARGET) $(OBJS) $(DEPS) \
	scanner.c scanner.h \
	parser.c parser.h parser.output \
	template.c template.h
