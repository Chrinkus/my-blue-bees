cmake_minimum_required(VERSION 3.16)

project(my-blue-bees VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

include_directories("/usr/include/SDL2")

add_subdirectory(src)

# Temporary until I decide to INSTALL
set(media_dirs
	"images"
	"fonts"
)

foreach(dir ${media_dirs})
	file(CREATE_LINK "${PROJECT_SOURCE_DIR}/${dir}"
		"${PROJECT_SOURCE_DIR}/build/${dir}" SYMBOLIC)
endforeach()

