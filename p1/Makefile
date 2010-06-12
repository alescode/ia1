# Macros
CC=g++
CFLAGS=
LDFLAGS=
ALGORITMOS := IDAestrella.o BFS.o
CLASES := Estado.o PerfilPreferencia.o
# Cuerpo

all: dodgson
dodgson: dodgson.cpp #$(CLASES) $(ALGORITMOS)
	$(CC) dodgson.cpp -o dodgson
$(ALGORITMOS): $(CLASES)
clean:
	rm -f *.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
