.PHONY: all clean compileServer createDynamicLib compileClient

all: clean compileServer createDynamicLib compileClient

clean:
	rm -f *.o *.so
	
compileServer:
	gcc -o servidor servidor.c implementacion.c array.c lista.c -lrt -lpthread

createDynamicLib:
	gcc -Wall -fPIC -c -lrt array.c
	ld -o libarray.so array.o -shared
	

compileClient:
	gcc -Wall -o cliente cliente.c -L. libarray.so -lrt
