CFLAGS = -std=c11

control: control.o model.o view.o

tags:
	ctags -R

clean:
	rm -f *.o control

.PHONY: tags clean
