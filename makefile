DX_PATH=C:\libraries\Microsoft_DirectX_SDK_June_2010

all:
	gcc -std=c++14 -c -I$(DX_PATH)\Include test.cpp -o test.o
	gcc -std=c++14 -c -I$(DX_PATH)\Include dx_init.cpp -o dx_init.o
	gcc -std=c++14 -c -I$(DX_PATH)\Include dx_app.cpp -o dx_app.o
	gcc -std=c++14 -c -I$(DX_PATH)\Include dx_obj.cpp -o dx_obj.o
	gcc -std=c++14 -c obj_loader.cpp -o obj_loader.o
	gcc test.o obj_loader.o dx_init.o dx_app.o dx_obj.o -o test.exe \
-L$(DX_PATH)\LIB -lstdc++ -lD3dx9 -lgdi32 -ld3d9 -lwinmm
	
clean:
	rm -rf *.o test.exe