cmake_minimum_required(VERSION 3.5.1)

project(HabitifyServer)

#############
### Setup ###
#############
#get parent directory
#cmake_path(GET "${PROJECT_SOURCE_DIR}" PARENT_PATH _habs_parent_dir)
get_filename_component(_habs_parent_dir ${PROJECT_SOURCE_DIR}/.. REALPATH) 
message(STATUS "PARENT DIR:::::: ${_habs_parent_dir}")
# set gRPC directories
set(glfw3_DIR ${CMAKE_CURRENT_BINARY_DIR}/../third_party/glfw/lib/cmake/glfw3)
set(Protobuf_DIR ${CMAKE_CURRENT_BINARY_DIR}/../third_party/protobuf/lib/cmake/protobuf)
set(c-ares_DIR ${CMAKE_CURRENT_BINARY_DIR}/../third_party/c-ares/lib/cmake/c-ares)
set(re2_DIR ${CMAKE_CURRENT_BINARY_DIR}/../third_party/re2/lib/cmake/re2)
set(ZLIB_ROOT ${CMAKE_CURRENT_BINARY_DIR}/../third_party/zlib)
set(absl_DIR ${CMAKE_CURRENT_BINARY_DIR}/../third_party/absl/lib/cmake/absl)
set(gRPC_DIR ${CMAKE_CURRENT_BINARY_DIR}/../third_party/grpc/lib/cmake/grpc)
# protoc files
get_filename_component(hab_proto "../utils/Habitify_protocol.proto" ABSOLUTE)
get_filename_component(hab_proto_path "${hab_proto}" PATH)
set(hab_proto_srcs "${_habs_parent_dir}/utils/Habitify_protocol.pb.cc")
set(hab_proto_hdrs "${_habs_parent_dir}/utils/Habitify_protocol.pb.h")
set(hab_grpc_srcs "${_habs_parent_dir}/utils/Habitify_protocol.grpc.pb.cc")
set(hab_grpc_hdrs "${_habs_parent_dir}/utils/Habitify_protocol.grpc.pb.h")

# includes
include(../cmake/common.cmake)
include_directories(${_habs_parent_dir}/utils)

message(STATUS "Hab_proto is:::::: ${hab_proto_path}")

# Generate Protoc Files
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

####################
### Build Server ###
####################
add_executable(${PROJECT_NAME}
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Server.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC
  "${PROJECT_SOURCE_DIR}/src"
  "${_habs_parent_dir}/utils"
  "${_habs_parent_dir}/build/third_party/protobuf/include"
  "${_habs_parent_dir}/build/third_party/grpc/include"
)

target_link_libraries(${PROJECT_NAME}
hab_grpc_proto
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
)

#################
### Libraries ###
#################
# hab_grpc_proto
add_library(hab_grpc_proto
  ${hab_grpc_srcs}
  ${hab_grpc_hdrs}
  ${hab_proto_srcs}
  ${hab_proto_hdrs})

target_include_directories(hab_grpc_proto PUBLIC
    "${_habs_parent_dir}/utils"
    "${_habs_parent_dir}/build/third_party/protobuf/include"
    "${_habs_parent_dir}/build/third_party/grpc/include"
)
  
target_link_libraries(hab_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

###############
### Targets ###
###############
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