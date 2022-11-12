cmake_minimum_required(VERSION 3.0.0)
project(Habitify)

set(OpenGL_GL_PREFERENCE GLVND)
#set(glfw3_DIR "${PROJECT_SOURCE_DIR}/build/external/glfw/src")

#find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/glfw")


add_executable(Habitify WIN32
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Application.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/Node.cpp"
    "${PROJECT_SOURCE_DIR}/src/Checklist.cpp")

#Dear ImGui
add_library(imgui
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_demo.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_draw.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_tables.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp"
    )

target_include_directories(imgui PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/external/imgui"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends"
    "${PROJECT_SOURCE_DIR}/external/glfw/include"
)

target_link_libraries(Habitify glfw ${GLFW_LIBRARIES})
target_link_libraries(Habitify OpenGL::GL)
target_link_libraries(Habitify imgui)

# set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wformat")

target_include_directories(Habitify PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src/"
    "${PROJECT_SOURCE_DIR}/src/utils"
    "${PROJECT_SOURCE_DIR}/external/imgui"
    "${PROJECT_SOURCE_DIR}/external/imgui/backends"
    "${PROJECT_SOURCE_DIR}/external/glfw/include"
)

#########
# Debug #
#########

set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")

target_compile_definitions(Habitify PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

###########
# Release #
###########

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")

target_compile_definitions(Habitify PRIVATE
    $<$<CONFIG:Release>:RELEASE>
)

##################
# RelWithDebInfo #
##################

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer -rdynamic")

target_compile_definitions(Habitify PRIVATE
    $<$<CONFIG:Release>:RELEASE>
)