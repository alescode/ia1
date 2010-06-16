# Macros
CC=g++
CFLAGS=-g
ALGORITMOS := IDAestrella.o BFS.o
CLASES := Estado.o Perfil.o Preferencia.o
# Cuerpo

all: dodgson
dodgson: dodgson.cpp $(CLASES) $(ALGORITMOS)
	$(CC) dodgson.cpp $(CLASES) $(ALGORITMOS) -o dodgson

$(ALGORITMOS): $(CLASES)
clean:
	rm -f dodgson *.o *.out *~

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
