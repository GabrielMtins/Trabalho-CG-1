GLFW_FLAGS := $(shell pkg-config --static --libs glfw3)
GL_FLAGS   := -lGL -lm
INCLUDE_DIR := -I./external/glm -I./external/glad/include -I./include
CCX_FLAGS := -std='c++17' -Wall -Wextra

all: app.o builder.o main.o object3d.o objecthandler.o shader.o glad.o
	g++ $(INCLUDE_DIR) $(CCX_FLAGS) app.o builder.o main.o object3d.o objecthandler.o shader.o glad.o -o main $(GL_FLAGS) $(GLFW_FLAGS)

app.o: src/App.cpp include/App.hpp
	g++ $(INCLUDE_DIR) $(CCX_FLAGS) $(GL_FLAGS) $(GLFW_FLAGS) -c src/App.cpp -o app.o

builder.o: src/Builder.cpp include/Builder.hpp
	g++ $(INCLUDE_DIR) $(CCX_FLAGS) $(GL_FLAGS) $(GLFW_FLAGS) -c src/Builder.cpp -o builder.o

main.o: src/main.cpp 
	g++ $(INCLUDE_DIR) $(CCX_FLAGS) $(GL_FLAGS) $(GLFW_FLAGS) -c src/main.cpp -o main.o

object3d.o: src/Object3d.cpp include/Object3d.hpp
	g++ $(INCLUDE_DIR) $(CCX_FLAGS) $(GL_FLAGS) $(GLFW_FLAGS) -c src/Object3d.cpp -o object3d.o

objecthandler.o: src/ObjectHandler.cpp include/ObjectHandler.hpp
	g++ $(INCLUDE_DIR) $(CCX_FLAGS) $(GL_FLAGS) $(GLFW_FLAGS) -c src/ObjectHandler.cpp -o objecthandler.o

shader.o: src/Shader.cpp include/Shader.hpp
	g++ $(INCLUDE_DIR) $(CCX_FLAGS) $(GL_FLAGS) $(GLFW_FLAGS) -c src/Shader.cpp -o shader.o

glad.o: external/glad/src/glad.c
	g++ -lm -std='c++17' -c -I./external/glad/include external/glad/src/glad.c -o glad.o $(GL_FLAGS) $(GLFW_FLAGS)

run:
	./main

clean:
	rm -f *.o main

