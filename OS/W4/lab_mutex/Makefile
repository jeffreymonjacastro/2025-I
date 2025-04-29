cc		:= gcc
cflags	:= -Wall
exec	:= atomic_inc compare-exchange ex0 ex1 ex2 inc mutex_lock mutex_sema spin
src		:= $(foreach name, $(exec), $(name).c)
asm		:= $(foreach name, $(exec), $(name).s)

all: $(exec)

asm: $(asm)

%.s: %.c
	$(cc) $< -o $@ -S $(cflags)
	

%: %.c
	$(cc) $< -o $@ $(cflags)


.PHONY: clean

clean:
	- rm $(exec) $(asm)
