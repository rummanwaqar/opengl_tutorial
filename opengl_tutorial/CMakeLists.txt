cmake_minimum_required (VERSION 2.6)
project (opengl_tutorial)

find_package( PkgConfig REQUIRED )
find_package( OpenGL REQUIRED )
find_package(GLEW REQUIRED)
pkg_check_modules( GLFW REQUIRED glfw3 )

add_definitions( -std=gnu++0x )

include_directories(
	include/
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
)

add_executable( helloWindow src/hello_window.cpp)
target_link_libraries( helloWindow 
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLEW_LIBRARIES}
	glfw
)

add_executable( helloTriangle src/hello_triangle.cpp)
target_link_libraries( helloTriangle 
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLEW_LIBRARIES}
	glfw
)

add_library( shader src/shader.cpp )
target_link_libraries( shader 
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLEW_LIBRARIES}
	glfw
)

add_executable( shaderTest src/shader_test.cpp)
target_link_libraries( shaderTest 
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLEW_LIBRARIES}
	glfw
	shader
)