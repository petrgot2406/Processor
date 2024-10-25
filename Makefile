CC = g++

HEADERS = Constants.h Struct.h StackFunc.h StackUtils.h StackHash.h ReadFromFile.h Stack.h

WINFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

assemble: Assembler.exe
	.\Assembler.exe

Assembler.exe: MainAssembler.o StackFunc.o StackUtils.o StackHash.o ReadFromFile.o
	$(CC) MainAssembler.o StackFunc.o StackUtils.o StackHash.o ReadFromFile.o -o Assembler.exe

process: Processor.exe
	.\Processor.exe

Processor.exe: MainProcessor.o StackFunc.o StackUtils.o StackHash.o ReadFromFile.o
	$(CC) MainProcessor.o StackFunc.o StackUtils.o StackHash.o ReadFromFile.o -o Processor.exe

MainAssembler.o: MainAssembler.cpp $(HEADERS)
	$(CC) -c MainAssembler.cpp $(WINFLAGS)

MainProcessor.o: MainProcessor.cpp $(HEADERS)
	$(CC) -c MainProcessor.cpp $(WINFLAGS)

ReadFromFile.o: ReadFromFile.cpp $(HEADERS)
	$(CC) -c ReadFromFile.cpp $(WINFLAGS)

StackFunc.o: StackFunc.cpp $(HEADERS)
	$(CC) -c StackFunc.cpp $(WINFLAGS)

StackUtils.o: StackUtils.cpp $(HEADERS)
	$(CC) -c StackUtils.cpp $(WINFLAGS)

StackHash.o: StackHash.cpp $(HEADERS)
	$(CC) -c StackHash.cpp $(WINFLAGS)
