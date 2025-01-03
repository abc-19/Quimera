#              ___
#           .="   "=._.---.		Sheet - Makefile
#         ."         c ' Y'`p	abc-19
#        /   ,       `.  w_/
#    jgs |   '-.   /     / 
#  _,..._|      )_-\ \_=.\
# `-....-'`------)))`=-'"`'"	Jan 02 2025
#

objs = main.o tui.o
opt = -O0
flags = -Wall -Wextra -Wpedantic $(opt)
cc = gcc
bin = quim

all: $(bin)

$(bin): $(objs)
	$(cc)	-o $@ $^

%.o: %.c
	$(cc)	$(flags) -c $^

clean:
	rm		-rf $(objs) $(bin) a.out
