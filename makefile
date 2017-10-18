FLAGS_GCC=-Wall -g -std=c11

CMPLR=gcc
CMPLR_FLAGS=$(FLAGS_GCC)

all:	db

db:	list.o tree.o utils.o db.c
	$(CMPLR) $(FLAGS_GCC) list.o tree.o utils.o db.c -o db

list.o: list.h list.c
	$(CMPLR) $(FLAGS_GCC) list.h list.c -c

tree.o: tree.h tree.c
	$(CMPLR) $(FLAGS_GCC) tree.h tree.c -c

utils.o: utils.h utils.c
	$(CMPLR) $(FLAGS_GCC) utils.h utils.c -c

test_list:
	$(CMPLR) $(CMPLR_FLAGS) -lcunit -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib test.c list.c

clean:
	rm -f *~
	rm -f *.o
	rm -f *.gch
	rm -f db

.PHONY:	all clean
