cmake_minimum_required(VERSION 3.5)
project(projeto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE all_SRCS
		"${PROJECT_SOURCE_DIR}/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/external/glad/src/*.c"
        )

add_executable(${PROJECT_NAME} ${all_SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall
    -Wextra
    -Wpedantic
)

include(FindPkgConfig)
pkg_search_module(GLFW REQUIRED glfw3)

find_library(OPENGL OpenGL)
find_library(MATH_LIBRARY m)

target_include_directories(${PROJECT_NAME} PRIVATE
	${GLFW_INCLUDE_DIRS}
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/external/glm"
	"${PROJECT_SOURCE_DIR}/external/glad/include"
	"${PROJECT_SOURCE_DIR}/external/stb/"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	${GLFW_LIBRARIES}
	${MATH_LIBRARY}
)
