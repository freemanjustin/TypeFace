###################################################################
#
# i like your type project
#
# freeman.justin@gmail.com
#
##################################################################


OBJ=	./src/main.o 

# lust with modules
INC=   -D_OS_X_ \
	-I./include 

CFLAGS=	-O3 -g -Wall

CC=	clang $(CFLAGS) $(INC)

# Libraries

# lust with modules
LFLAGS= 

# Executable

EXEC=	./bin/renderfont

$(EXEC):$(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LFLAGS)

clean:
	rm $(OBJ)
	rm $(EXEC)
