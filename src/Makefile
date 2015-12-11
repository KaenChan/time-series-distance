build_tsdistance:
	# gcc -fPIC -c dtw_c.c
	# gcc -g -shared -Wl,-soname,libdtw_c.so -o libdtw_c.so dtw_c.o
	gcc -fPIC -c dtw_c.cpp
	gcc -g -shared -Wl,-soname,libdtw_c.so -o libdtw_c.so dtw_c.o

clean:
	rm *.o *.so

