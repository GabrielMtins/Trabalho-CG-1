GLFW_FLAGS := $(shell pkg-config --static --libs glfw3)
GL_FLAGS   := -lGL

all: glad.o
	g++ -I./external/glm -I./external/glad/include -I./include -Wall -Wextra src/*.cpp glad.o -o main $(GL_FLAGS) $(GLFW_FLAGS)

glad.o: external/glad/src/glad.c
	g++ -lm -std='c++17' -c -I./external/glad/include external/glad/src/glad.c -o glad.o $(GL_FLAGS) $(GLFW_FLAGS)

run:
	./main

clean:
	rm -f glad.o main

