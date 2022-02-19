
#======================== Setup
CC := gcc
CFLAGS := -I./include
VPATH := src

SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,%.o,$(SOURCES)) 
DEPENDS := $(wildcard depends/*) -lgdi32

.PHONY : clean


#======================== Build
a.exe : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(DEPENDS) -o $@
	
%o : %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	del $(OBJECTS) a.exe