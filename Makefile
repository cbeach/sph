
OBJECTS :=\
	SPH.o\
	uVect.o\

SOURCE :=\
	./util/uVect.cpp\
	SPH.cpp


CC := g++
HEADERFILES := -I/home/owner/Documents/dev/cs406/SPH
LDFLAGS := -o
CPFLAGS := -c -g


INCLUDES := -lglut -lGL -lGLU

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) sph $(INCLUDES)

$(OBJECTS): $(SOURCE)
	$(CC) $(HEADERFILES) $(SOURCE) $(CPFLAGS)

clean:
	rm -f *.o
	
