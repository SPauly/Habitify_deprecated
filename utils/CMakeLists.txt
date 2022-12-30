cmake_minimum_required(VERSION 3.5.1)
project(utils)

include(../cmake/common.cmake)
  
# Proto file
get_filename_component(hab_proto "Habitify_protocol.proto" ABSOLUTE)
get_filename_component(hab_proto_path "${hab_proto}" PATH)

# Generated sources
set(hab_proto_srcs "${PROJECT_SOURCE_DIR}/Habitify_protocol.pb.cc")
set(hab_proto_hdrs "${PROJECT_SOURCE_DIR}/Habitify_protocol.pb.h")
set(hab_grpc_srcs "${PROJECT_SOURCE_DIR}/Habitify_protocol.grpc.pb.cc")
set(hab_grpc_hdrs "${PROJECT_SOURCE_DIR}/Habitify_protocol.grpc.pb.h")

add_custom_command(
      OUTPUT "${hab_proto_srcs}" "${hab_proto_hdrs}" "${hab_grpc_srcs}" "${hab_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${PROJECT_SOURCE_DIR}"
        --cpp_out "${PROJECT_SOURCE_DIR}"
        -I "${hab_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hab_proto}"
      DEPENDS "${hab_proto}")

include_directories("${PROJECT_SOURCE_DIR}")

# hab_grpc_proto
add_library(hab_grpc_proto
  ${hab_grpc_srcs}
  ${hab_grpc_hdrs}
  ${hab_proto_srcs}
  ${hab_proto_hdrs})

target_include_directories(hab_grpc_proto PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../build/third_party/protobuf/include"
    "${PROJECT_SOURCE_DIR}/../build/third_party/grpc/include"
)
  
target_link_libraries(hab_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})