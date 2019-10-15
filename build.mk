INCDIR = -Idepend/ -Isource/_head/
LIBDIR = -Ldepend/
GLAD = output/glad.o
OBJS = output/helperopengl.o output/helperopenglsdl.o

build: \
output/demo_opengl_openwindow.out \
output/demo_opengl_triangle.out \
output/glad.o \
output/helperopenglsdl.o \
output/helperopengl.o

clean:
	rm $(wildcard output/*)

window: build
	./output/demo_opengl_openwindow.out

triangle: build
	./output/demo_opengl_triangle.out

output/glad.o: source/glad/glad.c
	gcc $(INCDIR) $(LIBDIR) -c source/glad/glad.c -o output/glad.o

output/helperopenglsdl.o: source/opengl/helperopenglsdl.c
	gcc $(INCDIR) $(LIBDIR) -c source/opengl/helperopenglsdl.c -o output/helperopenglsdl.o

output/helperopengl.o: source/opengl/helperopengl.c
	gcc $(INCDIR) $(LIBDIR) -c source/opengl/helperopengl.c -o output/helperopengl.o

output/demo_opengl_openwindow.out: source/demo/demo_opengl_openwindow.c $(GLAD)
	gcc $(INCDIR) $(LIBDIR) source/demo/demo_opengl_openwindow.c $(GLAD) -lSDL2 -lGL -ldl -o output/demo_opengl_openwindow.out

output/demo_opengl_triangle.out: source/demo/demo_opengl_triangle.c $(GLAD) $(OBJS) 
	gcc $(INCDIR) $(LIBDIR) source/demo/demo_opengl_triangle.c $(GLAD) $(OBJS) -lSDL2 -lGL -ldl -o output/demo_opengl_triangle.out