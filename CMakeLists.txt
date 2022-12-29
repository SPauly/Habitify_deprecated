#########################################
##### Build Protoc Files + Habitify #####
#########################################
cmake_minimum_required(VERSION 3.5.1)
# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wformat")
set(-DCMAKE_RC_COMPILER=windres)

project(Habitify)

include(ExternalProject)

# Build variables
option(_HAB_INITIAL_BUILD "Build all the third party projects" ON)
option(_HAB_FULL_BUILD "Build app and server" OFF)
option(_HAB_APP_BUILD "Build the app" OFF)
option(_HAB_SERVER_BUILD "Build the server" OFF)
option(_HAB_CREATE_PROTOFILES "Create the Protofiles" ON)

# Set Variables ---> edit here for debuging
set(_HAB_INITIAL_BUILD OFF CACHE BOOL "" FORCE)
set(_HAB_FULL_BUILD ON CACHE BOOL "" FORCE)
set(_HAB_APP_BUILD OFF CACHE BOOL "" FORCE)
set(_HAB_SERVER_BUILD ON CACHE BOOL "" FORCE)
if(_HAB_FULL_BUILD)
  set(_HAB_APP_BUILD ON CACHE BOOL "" FORCE)
  set(_HAB_SERVER_BUILD ON CACHE BOOL "" FORCE)
endif()
set(_HAB_CREATE_PROTOFILES OFF CACHE BOOL "" FORCE)

# Build third_party_dependencies
if(_HAB_INITIAL_BUILD)
  ExternalProject_Add(Third_Party_Depends
    PREFIX external
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external"
    BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/third_party"
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/Third_Party_Depends
    )
    
endif()

set(glfw3_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw/lib/cmake/glfw3)
set(Protobuf_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/protobuf/lib/cmake/protobuf)
set(c-ares_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/c-ares/lib/cmake/c-ares)
set(re2_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/re2/lib/cmake/re2)
set(ZLIB_ROOT ${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib)
set(absl_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/absl/lib/cmake/absl)
set(gRPC_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/grpc/lib/cmake/grpc)


###################
### Proto Files ###
###################
if(_HAB_APP_BUILD)
  include(cmake/common.cmake)
  
  # Proto file
  get_filename_component(hab_proto "utils/Habitify_protocol.proto" ABSOLUTE)
  get_filename_component(hab_proto_path "${hab_proto}" PATH)
  
  # Generated sources
  set(hab_proto_srcs "${PROJECT_SOURCE_DIR}/utils/Habitify_protocol.pb.cc")
  set(hab_proto_hdrs "${PROJECT_SOURCE_DIR}/utils/Habitify_protocol.pb.h")
  set(hab_grpc_srcs "${PROJECT_SOURCE_DIR}/utils/Habitify_protocol.grpc.pb.cc")
  set(hab_grpc_hdrs "${PROJECT_SOURCE_DIR}/utils/Habitify_protocol.grpc.pb.h")
  
  if(_HAB_CREATE_PROTOFILES)
  add_custom_command(
        OUTPUT "${hab_proto_srcs}" "${hab_proto_hdrs}" "${hab_grpc_srcs}" "${hab_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${PROJECT_SOURCE_DIR}/utils"
          --cpp_out "${PROJECT_SOURCE_DIR}/utils"
          -I "${hab_proto_path}"
          --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
          "${hab_proto}"
        DEPENDS "${hab_proto}")
  endif()
  
  include_directories("${PROJECT_SOURCE_DIR}/utils")
  
  # hab_grpc_proto
  add_library(hab_grpc_proto
    ${hab_grpc_srcs}
    ${hab_grpc_hdrs}
    ${hab_proto_srcs}
    ${hab_proto_hdrs})
  
  target_include_directories(hab_grpc_proto PUBLIC
      "${PROJECT_SOURCE_DIR}/utils"
      "${PROJECT_SOURCE_DIR}/build/third_party/protobuf/include"
      "${PROJECT_SOURCE_DIR}/build/third_party/grpc/include"
  )
    
  target_link_libraries(hab_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
  
  ################
  ### Habitify ###
  ################
  set(OpenGL_GL_PREFERENCE GLVND)
  
  find_package(glfw3 3.3 REQUIRED)
  find_package(OpenGL REQUIRED)
  
  add_executable(${PROJECT_NAME}
      "${PROJECT_SOURCE_DIR}/src/main.cpp"
      "${PROJECT_SOURCE_DIR}/src/Application.cpp"
      "${PROJECT_SOURCE_DIR}/src/Board.cpp"
      "${PROJECT_SOURCE_DIR}/src/utils/Node.cpp"
      "${PROJECT_SOURCE_DIR}/src/utils/LayerStack.cpp"
      "${PROJECT_SOURCE_DIR}/src/Checklist.cpp"
  )
  
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
      "${PROJECT_SOURCE_DIR}/external/imgui"
      "${PROJECT_SOURCE_DIR}/external/imgui/backends"
      "${PROJECT_SOURCE_DIR}/build/third_party/glfw/include"
  )
  
  target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} OpenGL::GL)
  target_link_libraries(${PROJECT_NAME} imgui)
  target_link_libraries(${PROJECT_NAME}
  hab_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
  
  target_include_directories(${PROJECT_NAME} PUBLIC
      "${PROJECT_SOURCE_DIR}"
      "${PROJECT_SOURCE_DIR}/utils"
      "${PROJECT_SOURCE_DIR}/src/"
      "${PROJECT_SOURCE_DIR}/src/utils"
      "${PROJECT_SOURCE_DIR}/external/imgui"
      "${PROJECT_SOURCE_DIR}/external/imgui/backends"
      "${PROJECT_SOURCE_DIR}/build/thir_party/glfw/include/"
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

endif()

if(_HAB_SERVER_BUILD)
  ExternalProject_Add(HabitifyServer
        PREFIX HabitifyServer
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/HabitifyServer"
        BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/HabitifyServer"
        INSTALL_COMMAND ""
        BUILD_ALWAYS TRUE
        CMAKE_CACHE_ARGS
          -D_HAB_CREATE_PROTOFILES:BOOL=_HAB_CREATE_PROTOFILES
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/HabitifyServer
)
endif()