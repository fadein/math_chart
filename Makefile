CFLAGS = -std=gnu11 -ggdb3

worksheet: control.o model.o view.o worksheet.o

tags:
	ctags -R

clean:
	rm -f *.o worksheet

deepclean: clean
	rm -f core* tags

.PHONY: tags clean
