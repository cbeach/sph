OBJECTS :=\
	main.o\
	uVect.o\
	sp.o\
	sph.o\
	ogl.o

SOURCE :=\
	./util/uVect.cpp\
	./render/ogl.cpp\
	./particle/sp.cpp\
	./particle/sph.cpp\
	main.cpp
 
 
 
 
CC := g++
HEADERFILES := -I/home/casey/Documents/cs406/sph/ -I/usr/include/boost/
LDFLAGS := -o
CPFLAGS := -O3 -c -g


INCLUDES := -lglut -lGL -lGLU

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) sph $(INCLUDES)

$(OBJECTS): $(SOURCE)
	$(CC) $(HEADERFILES) $(SOURCE) $(CPFLAGS)

clean:
	rm -f *.o
         


