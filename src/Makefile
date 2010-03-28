### Initially taken fromhttp://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
### and then modified ever since as desired by Neeraj Prashar

# -Wall gives out warnings 
CC=g++ -Wall 
#include directory
IDIR =../include
CFLAGS=-I$(IDIR)
#object directory
ODIR =obj
#library directory
LDIR =../lib
#linking math, objective-c library(not needed - so removed) and OpenGL, and GLUT framework. 
LIBS=-lm -framework OpenGL -framework GLUT
_DEPS=fileparser.h vec.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ=PPM.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

floader: $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test: test.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Makefile doesn't do anything to a file called clean 
.PHONY: clean

clean: 
		rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
