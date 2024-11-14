# Variables
GCC = gcc
CFLAGS = -c 

# Target principal
all: publicador suscriptor sistema

# Reglas de compilación
publicador: publicador
	$(GCC) $(CFLAGS) publicador.c 
	$(GCC) -o publicador publicador.o mainPublicador.c
	$(RM) *.o
suscriptor: suscriptor
	$(GCC) $(CFLAGS) suscriptor.c 
	$(GCC) -o suscriptor suscriptor.o mainSuscriptor.c
	$(RM) *.o
sistema: sistema
	$(GCC) $(CFLAGS) sistema.c 
	$(GCC) -o sistema sistema.o mainSistema.c
	$(RM) *.o
# Limpieza
clean:
	rm -f publicador suscriptor sistema
