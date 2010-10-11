
OBJECTS :=\
	SPH.o\
	uVect.o\
	sp.o\
	ogl.o

SOURCE :=\
	./util/uVect.cpp\
	./render/ogl.cpp\
	./particle/sp.cpp\
	SPH.cpp
	



CC := g++
HEADERFILES := -I/home/casey/Documents/cs406/sph/
LDFLAGS := -o
CPFLAGS := -c -g


INCLUDES := -lglut -lGL -lGLU

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) sph $(INCLUDES)

$(OBJECTS): $(SOURCE)
	$(CC) $(HEADERFILES) $(SOURCE) $(CPFLAGS)

clean:
	rm -f *.o
	
