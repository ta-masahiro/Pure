PROGRAM := vm-test
OBJS := hash.o vector.o read.o vvm.o

CC := gcc
CFLAGS := -g -O0
LDLIBS :=  -lgc

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $^ $(LDLIBS)

$(OBJS): vm.h

clean:
	$(RM) *.o
	$(RM) vm-test
