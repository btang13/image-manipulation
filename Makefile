CC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
LDLIBS = -lm
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

#this links the project executable
project: project.o ppm_io.o imageManip.o
	$(CC) -o project project.o ppm_io.o imageManip.o $(LDLIBS)

#this compiles ppm_io.c into an object file
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

#this compiles imageManip.c into an object file
imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c imageManip.c

#this compiles project.c into an object file
project.o: project.c ppm_io.h imageManip.h
	$(CC) $(CFLAGS) -c project.c

#the command "make clean" will remove intermediate and executable files
clean:
	rm -f *.o project
