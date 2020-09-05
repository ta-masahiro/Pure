PROGRAM := vm-test
OBJS := hash.o vector.o read.o vm.o

CC := gcc
#FLAGS := -g -O0
FLAGS := -O3
LDLIBS :=  -lgc -lgmp

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(FLAGS) $^ $(LDLIBS)

$(OBJS): vm.h

.SUFFIXES: .c .o
.c.o:
	$(CC) $(FLAGS) -c $<
clean:
	$(RM) *.o
	$(RM) vm-test
