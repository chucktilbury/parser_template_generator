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
		trace.o

HEADERS = trace.h \
		errors.h \
		common.h

#CARGS = -Wall -DUSE_TRACE -g
CARGS = -Wall -g
CC = clang

%.o:%.c
	$(CC) $(CARGS) -c -o $@ $<

$(TARGET): $(OBJS) $(DEPS)
	$(CC) -g -o $(TARGET) $(OBJS)

scanner.c scanner.h: scanner.l
	flex scanner.l

parser.c parser.h: parser.y
	bison parser.y

$(DEPS): $(OBJS:.o=.c)
	$(CC) -MM $^ > $(DEPS)

include $(DEPS)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) scanner.c scanner.h parser.c parser.h parser.output