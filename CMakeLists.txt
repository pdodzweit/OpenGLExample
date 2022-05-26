# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (Tutorials)

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory (external)

include_directories(
	external/glfw-2.7.2/include/
	external/glm-0.9.1/
	external/glew-1.5.8/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_272
	GLEW_158
)

add_definitions(
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)


# Tutorial 5
add_executable(tutorial05_textured_cube
	tutorial05_textured_cube/tutorial05.cpp
	common/shader.cpp
	common/shader.hpp
	common/texture.cpp
	common/texture.hpp
)
target_link_libraries(tutorial05_textured_cube
	${ALL_LIBS}
)
add_custom_command(
   TARGET tutorial05_textured_cube POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial05_textured_cube${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial05_textured_cube/"
)

add_custom_command(
        TARGET tutorial05_textured_cube POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/tutorial05_textured_cube/vert.glsl
                $<TARGET_FILE_DIR:tutorial05_textured_cube>/vert.glsl)

add_custom_command(
        TARGET tutorial05_textured_cube POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/tutorial05_textured_cube/tcs.glsl
                $<TARGET_FILE_DIR:tutorial05_textured_cube>/tcs.glsl)
				
add_custom_command(
        TARGET tutorial05_textured_cube POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/tutorial05_textured_cube/tes.glsl
                $<TARGET_FILE_DIR:tutorial05_textured_cube>/tes.glsl)
				
add_custom_command(
        TARGET tutorial05_textured_cube POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/tutorial05_textured_cube/frag.glsl
               $<TARGET_FILE_DIR:tutorial05_textured_cube>/frag.glsl)
			   
set_target_properties(
    tutorial05_textured_cube PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:tutorial05_textured_cube>)


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )

