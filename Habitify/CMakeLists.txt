#########################################
##### Build Protoc Filse + Habitify #####
#########################################
cmake_minimum_required(VERSION 3.5.1)
project(Habitify)
include(cmake/common.cmake)

# Proto file
get_filename_component(hw_proto "../utils/Habitify_protocol.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/utils/Habitify_protocol.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/utils/Habitify_protocol.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/utils/Habitify_protocol.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/utils/Habitify_protocol.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/utils"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/utils"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}/utils")

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
  
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

####### Habitify
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/glfw")

add_executable(${PROJECT_NAME} WIN32
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Application.cpp"
    "${PROJECT_SOURCE_DIR}/src/Board.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/Node.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/LayerStack.cpp"
    "${PROJECT_SOURCE_DIR}/src/Checklist.cpp"
)

add_dependencies(${PROJECT_NAME} grpc)

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

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME}
hw_grpc_proto
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF})

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_BINARY_DIR}/utils"
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

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

###########
# Release #
###########

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:RELEASE>
)

##################
# RelWithDebInfo #
##################

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer -rdynamic")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:RELEASE>
)