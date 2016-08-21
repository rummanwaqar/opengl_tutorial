cmake_minimum_required (VERSION 2.6)
project (opengl_tutorial)

find_package( PkgConfig REQUIRED )
find_package( OpenGL REQUIRED )
find_package(GLEW REQUIRED)
pkg_check_modules( GLFW REQUIRED glfw3 )

include_directories(
	include/
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
)

add_executable(test1 src/test.cpp)
target_link_libraries(test1 
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLEW_LIBRARIES}
)