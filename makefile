fractal_window: resource.o fractal.o fractal_window.o
	gcc -g -o fractal_window.exe  fractal.o fractal_window.o resource.o -lgdi32 -lm -lpthread
resource.o:resource.rc resource.h icon1.ico
	windres resource.rc resource.o
fractal.o:fractal.c makefile
	gcc -g -c fractal.c -std=c99 -O10
fractal_window.o:fractal_window.c makefile
	gcc -g -c fractal_window.c -std=c99 -O10
clean:
	-del *.o