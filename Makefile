CFLAGS = -std=gnu11 -ggdb3

test: control.o model.o view.o test.o

tags:
	ctags -R

clean:
	rm -f *.o test

deepclean: clean
	rm -f core* tags

.PHONY: tags clean
