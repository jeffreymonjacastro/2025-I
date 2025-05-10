cc		:= gcc
cflags	:= -Wall -g
exec	:= r-w-broadcast r-w-sequential r-w-sol-example r-w-starvation
src		:= $(foreach name, $(exec), $(name).c)

all: $(exec)

%: %.c
	$(cc) $< -o $@ $(cflags)


.PHONY: clean

clean:
	- rm $(exec)
