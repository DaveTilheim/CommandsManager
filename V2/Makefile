CC = g++
LD = g++
STD = -std=c++2a
EXE = cman_d
OBJS = Command.o Prototype.o Tokens.o CommandException.o Prompt.o main.o

all : $(EXE)

$(EXE) : $(OBJS)
	$(LD) -o $(EXE) $(OBJS) $(STD)

Command.o : Command.hpp Command.cpp
	$(CC) -o Command.o -c Command.cpp $(STD)

Prototype.o : Prototype.hpp Prototype.cpp
	$(CC) -o Prototype.o -c Prototype.cpp $(STD)

Tokens.o : Tokens.hpp Tokens.cpp
	$(CC) -o Tokens.o -c Tokens.cpp $(STD)

CommandException.o : CommandException.hpp CommandException.cpp
	$(CC) -o CommandException.o -c CommandException.cpp $(STD)

Prompt.o : Prompt.hpp Prompt.cpp
	$(CC) -o Prompt.o -c Prompt.cpp $(STD)

main.o : main.cpp
	$(CC) -o main.o -c main.cpp $(STD)

clean :
	@rm -f *.o
	@rm -f $(EXE)
