INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/lib/

NAME = main
COMPILERFLAGS = -Wall -I
CC = gcc
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm -lstdc++

all: main
main: $(NAME).o
	$(CC) $(CFLAGS) -o $@ $(LIBDIR) $< $(LIBRARIES)

.PHONY : clean
clean :
	-rm $(NAME) $(NAME).o
