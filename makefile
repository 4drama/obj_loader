all:
	gcc -std=c++14 -c test.cpp -o test.o
	gcc -std=c++14 -c obj_loader.cpp -o obj_loader.o
	gcc test.o obj_loader.o -o test.exe -lstdc++
	
clean:
	rm -rf *.o test.exe