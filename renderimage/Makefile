###################################################################
#
# freeman.justin@gmail.com
#
##################################################################


OBJ=	./src/main.o \
	./src/cli.o

# Compliler flags
INC=   -D_OS_X_ -I./include 

CFLAGS=	-O3 -g -Wall 

CC=	gcc $(CFLAGS) $(INC) 

# Libraries

LFLAGS=

# Executable

EXEC=	./bin/renderimage	

$(EXEC):$(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LFLAGS)

clean:
	rm $(OBJ)
	rm $(EXEC)
