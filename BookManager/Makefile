bookmanager.out : bookprogram.o memberprogram.o client.o
	gcc -o c09.out bookprogram.o memberprogram.o client.o

bookprogram.o : bookprogram.c
	gcc -c -o bookprogram.o bookprogram.c

memberprogram.o : memberprogram.c
	gcc -c -o memberprogram.o memberprogram.c

client.o : client.c
	gcc -c -o client.o client.c

clean:
	rm *.o bookmanager.out
