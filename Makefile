CFLAGS = -std=c11 -ggdb3

control: control.o model.o view.o main.o

tags:
	ctags -R

clean:
	rm -f *.o control

deepclean: clean
	rm -f core tags

.PHONY: tags clean
