TARGET = pgen
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

CARGS = -Wall -DUSE_TRACE -g
CC = clang

%.o:%.c
	$(CC) $(CARGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS)

scanner.o: scanner.c scanner.h
scanner.c scanner.h: scanner.l
	flex scanner.l

parser.o: parser.c parser.h
parser.c parser.h: parser.y
	bison parser.y

clean:
	$(RM) $(TARGET) $(OBJS) scanner.c scanner.h parser.c parser.h parser.output