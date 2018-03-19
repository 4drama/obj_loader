DX_PATH=C:\libraries\Microsoft_DirectX_SDK_June_2010

all:
	gcc -std=c++14 -c -I$(DX_PATH)\Include test.cpp -o test.o
	gcc -std=c++14 -c -I$(DX_PATH)\Include dx_init.cpp -o dx_init.o
	gcc -std=c++14 -c obj_loader.cpp -o obj_loader.o
	gcc test.o obj_loader.o dx_init.o -o test.exe -L$(DX_PATH)\LIB -lstdc++ -lD3dx9 -lgdi32
	
clean:
	rm -rf *.o test.exe