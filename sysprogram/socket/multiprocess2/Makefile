src=$(wildcard *.c)
obj=$(patsubst %.c,%.o,$(src))

all:$(obj)
	@gcc $^ -o app
%.o:%.c
	gcc -c $< -o $@

.PHONY:clean
clean:
	@rm -f $(obj)  app
