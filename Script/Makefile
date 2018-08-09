.PHONY:Default clean

BIN := mp3

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

Default:Print $(BIN)
	@echo "-----------------------------------------------------------------" 
	@echo "                      Good job!  ~(@^_^@)~" 
	@echo "-----------------------------------------------------------------" 

Print:
	@echo "SRCS : $(SRCS)" 
	@echo "OBJS : $(OBJS)"

$(BIN):$(OBJS)
	gcc -o $@ $^

%.o:%.c
	gcc -o $@ -c $^

clean:
	rm $(BIN) *.o
