all: main.o get_web_object.o 
	gcc -o get_web_object main.o get_web_object.o

main.o: main.c main.h
	gcc -c main.c

get_web_object.o: get_web_object.c get_web_object.h
	gcc -c get_web_object.c

clean: 
	rm get_web_object get_web_object.o main.o
