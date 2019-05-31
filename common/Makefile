CF  = -O3 -Wall -Wshadow -pedantic -std=gnu99 -g
CC  = gcc $(CF)
OBJ = eprintf.o tree.o lex.yy.o y.tab.o
LEXOR		=	newick.l
GRAMMAR		=	newick.y
libcommon.a: $(OBJ)
	ar -cvq $@ $^
	ranlib $@

tree.o: y.tab.h
y.tab.h: y.tab.c
lex.yy.c: y.tab.c $(LEXOR)
	flex $(LEXOR)

y.tab.c: $(GRAMMAR)
	bison -y -d $(GRAMMAR)

.PHONY: clean
clean:
	rm -f *.o libcommon.a
