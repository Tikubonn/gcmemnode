
export CFLAGS = -I. -Iinclude -Llib

debug: .always 
	make libgcmemnode.a libgcmemnode.so CFLAGS="$(CFLAGS) -Og -g -Wall"

release: .always 
	make libgcmemnode.a libgcmemnode.so CFLAGS="$(CFLAGS) -O3 -Wall"

test: .always
	make debug 
	make test.exe CFLAGS="$(CFLAGS) -Og -g"
	gdb test.exe 
	#./test.exe

setup: .always
	git clone https://github.com/tikubonn/bitarray
	git clone https://github.com/tikubonn/memnode
	make release -C bitarray
	cp bitarray/bitarray.h include/bitarray.h
	cp bitarray/libbitarray.so lib/libbitarray.so
	cp bitarray/libbitarray.a lib/libbitarray.a
	make setup release -C memnode
	cp memnode/memnode.h include/memnode.h
	cp memnode/libmemnode.so lib/libmemnode.so
	cp memnode/libmemnode.a lib/libmemnode.a

clean: .always
	rm -f gcmemnode.o

libbitarray.so: lib/libbitarray.so
	cp lib/libbitarray.so libbitarray.so

libmemnode.so: lib/libmemnode.so
	cp lib/libmemnode.so libmemnode.so

test.exe: test.c libgcmemnode.so gcmemnode.h libmemnode.so libbitarray.so
	gcc $(CFLAGS) test.c libgcmemnode.so libmemnode.so libbitarray.so -o test.exe 

.always:

gcmemnode.o: gcmemnode.c gcmemnode.h 
	gcc $(CFLAGS) -c gcmemnode.c -o gcmemnode.o

libgcmemnode.so: gcmemnode.o lib/libbitarray.so lib/libmemnode.so
	gcc $(CFLAGS) -shared gcmemnode.o lib/libbitarray.so lib/libmemnode.so -o libgcmemnode.so

libgcmemnode.a: gcmemnode.o 
	ar r libgcmemnode.a gcmemnode.o
