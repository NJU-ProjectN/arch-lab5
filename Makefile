CC=gcc
CSRC=$(shell find . -name '*.c')
OBJS=$(CSRC:.c=.o)

CFLAGS=-m32 -O2 -Wall -Werror 
DEPEND=$(CC) -MM -I.

my-oj: $(OBJS)
	$(CC) -o my-oj $(CFLAGS) $(OBJS)

clean:
	-rm my-oj *.o *.d

%.d:%.c Makefile
	@$(SHELL) -ec '$(DEPEND) $< | sed '\''s/\($*\.o\)[ :]*/\1 $@ : /g'\'' > $@'

-include $(CSRC:.c=.d)
