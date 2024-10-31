CC = g++

HEADERS = Constants.h Struct.h StackFunc.h StackUtils.h StackHash.h ReadFromFile.h Stack.h

WINFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

LINUXFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


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