obj-m += assignment3.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} modules
	gcc -o producer producer.c
	gcc -o consumer consumer.c 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} clean
