NAME = main

ifdef OS #OS is only set on Windows, so we can use it to detect whether we're on Windows or *nix.
	RM = del /Q
	INCLUDE = "C:\MinGW\include\"
	LIBDIR  = "C:\MinGW\lib\"
	LIBRARIES = -lm -lstdc++
	TARGETS = $(NAME).exe

else
	ifeq ($(shell uname), Linux)
		RM = rm -f
		INCLUDE = -I/usr/include/
		LIBDIR  = -L/usr/lib/
		LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm -lstdc++
		TARGETS = $(NAME)

	endif
endif

COMPILERFLAGS = -Wall -I
CC = gcc
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)


all: main
main:
	$(CC) $(NAME).cc $(CFLAGS) -o $(TARGETS) $(LIBDIR) $(LIBRARIES)
	
.PHONY : clean
clean :
	RM $(TARGETS) $(NAME).o
