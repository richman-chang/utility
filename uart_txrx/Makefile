CROSS_COMPILE=arm-linux-
all:
	gcc -o tx86 tx.c
	gcc -o rx86 rx.c
	$(CROSS_COMPILE)gcc -o txarm tx.c
	$(CROSS_COMPILE)gcc -o rxarm rx.c
	

clean:
	rm -rf tx rx
