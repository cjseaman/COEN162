all: main.o get_web_object.o 
	gcc -o middleman main.o get_web_object.o

test: testmain.o get_web_object.o
	gcc -o test testmain.o get_web_object.o

testmain.o: testmain.c get_web_object.h
	gcc -c testmain.c

main.o: main.c get_web_object.h
	gcc -c main.c

get_web_object.o: get_web_object.c get_web_object.h
	gcc -c get_web_object.c

clean: 
	rm -f middleman test testmain.o get_web_object.o main.o
