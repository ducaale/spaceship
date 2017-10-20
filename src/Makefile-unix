CC:=clang++-4.0
CFLAGS=-Wall -ggdb -std=c++1z
LDFLAGS=
LIBS=-lsfml-graphics -lsfml-window -lsfml-system
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=game

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

-include $(OBJECTS:.o=.d)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $*.o
	$(CC) -MM $(CFLAGS) $*.cpp -o $*.d

clean:
	rm -f $(EXECUTABLE) *.o *.d

