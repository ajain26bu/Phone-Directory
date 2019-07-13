all: ajain26_skolwan1_phone_directory.out
	gcc ajain26_skolwan1_phone_directory.o -o ajain26_skolwan1_phone_directory.out

ajain26_skolwan1_phone_directory.out:
		gcc -c ajain26_skolwan1_phone_directory.c

clean:
	rm *.o ajain26_skolwan1_phone_directory.out

