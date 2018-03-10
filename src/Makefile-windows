CC=g++
CFLAGS=-Wall -ggdb -std=c++1z
LDFLAGS=
LIBS=-DSFML_STATIC -static -lpthread -static-libgcc -static-libstdc++ -lmingw32 -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lopengl32 -lfreetype -ljpeg -lwinmm -lgdi32 -lopenal32 -lFLAC -lvorbisenc -lvorbisfile -lvorbis -logg

INCLUDE_PATHS=-IC:\sfml-2.4-static\include
LIBRARY_PATHS=-LC:\sfml-2.4-static\lib

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=game.exe

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBRARY_PATHS) $(INCLUDE_PATHS) $(LIBS)

-include $(OBJECTS:.o=.d)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE_PATHS) -DSFML_STATIC $*.cpp -o $*.o
	$(CC) -MM $(CFLAGS) $(INCLUDE_PATHS) $*.cpp -o $*.d

clean:
	del $(EXECUTABLE) *.o *.d

