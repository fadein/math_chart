worksheet_BIN  := worksheet
worksheet_SRCS := $(wildcard *.c)

CFLAGS = -std=gnu11 -ggdb3

#TODO- a deps file so that I can automagically rebuild when a header changes
%.d: $(worksheet_SRCS)
	@ $(CC) $(CPPFLAGS) -MM $*.c | sed -e 's@^\(.*\)\.o:@\1.d \1.o:@' > $@

$(worksheet_BIN): ${worksheet_SRCS:.c=.o}

tags:
	ctags -R

clean:
	rm -f *.d *.o worksheet

deepclean: clean
	rm -f core* tags

.PHONY: tags clean


# Include the list of dependancies generated for each object file
ifneq "$(MAKECMDGOALS)" "clean"
-include ${worksheet_SRCS:.c=.d}
endif
