###############################################################################
#	SPECIAL MACROS
#
#	CC	-	command name of the compiler
#	CFLAGS	-	flags to be passed to the compiler
#	$@	-	full name of the current target (allows for recursion)
#	$?	-	list of dependency files that are out of date
#	$<	-	name of the current dependency
###############################################################################

#select compiler
CC=clang++
#compile flags
CFLAGS=-Wall -std=c++11
#non-standard library path
#DIRS=
DIRS=-I ~/SFML-2.2/include
#additional libraries
LIBS=-lsfml-system -lsfml-graphics -lsfml-window 
#executable name
EX=a.out
#sources
SOURCES=main.cpp Menu.cpp
#headers
HEADERS=Menu.h
#source objects
OBJECTS=$(SOURCES:.cpp=.o)


#build all
all: $(SOURCES) $(EX) $(HEADERS)

#build for debugging
debug: CFLAGS += -ggdb
debug: all

#build for release
release: all

#build the executable
$(EX): $(OBJECTS)
	$(CC) -o $(EX) $(OBJECTS) $(CFLAGS) $(LIBS) $(DIRS)

#recursively build all sources
.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(DIRS)

clean:
	rm -rf *.o $(EX)
