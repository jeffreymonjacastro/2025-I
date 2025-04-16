cc		:= gcc
cflags	:= -Wall
exec	:= fork-6k pthread-6k matmul-threads matmul-process miturno miturno2 barrier
src		:= $(foreach name, $(exec), $(name).c)

all: $(exec)

%: %.c
	$(cc) $< -o $@ $(cflags)


.PHONY: clean

clean:
	- rm $(exec)
