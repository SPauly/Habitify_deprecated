#Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake "superbuild" file for C++ helloworld example.
# This build file demonstrates how to build the helloworld project
# and all its dependencies in a single cmake build (hence "superbuild")
# that is easy to build and maintain.
# cmake's ExternalProject_Add() is used to import all the sub-projects,
# including the "helloworld" project itself.
# See https://blog.kitware.com/cmake-superbuilds-git-submodules/

cmake_minimum_required(VERSION 3.5.1)

# Project
project(Habitify-SuperBuild)

include(ExternalProject)

# Build variables
#option(_HAB_FULL_BUILD "Build all the third party projects + app and server" ON)
#option(_HAB_NO_THIRD_BUILD "Build app and server" OFF)
#option(_HAB_APP_BUILD "Build the app" OFF)
#option(_HAB_SERVER_BUILD "Build the server" OFF)
#option(_HAB_CREATE_PROTOFILES_var "Create the Protofiles" ON)
#
## Set Variables ---> edit here for debuging
#set(_HAB_FULL_BUILD ON CACHE BOOL "" FORCE)
#set(_HAB_NO_THIRD_BUILD OFF CACHE BOOL "" FORCE)
#set(_HAB_APP_BUILD OFF CACHE BOOL "" FORCE)
#set(_HAB_SERVER_BUILD OFF CACHE BOOL "" FORCE)
#if(_HAB_NO_THIRD_BUILD)
#  set(_HAB_APP_BUILD ON CACHE BOOL "" FORCE)
#  set(_HAB_SERVER_BUILD ON CACHE BOOL "" FORCE)
#endif()
#set(_HAB_CREATE_PROTOFILES_var ON CACHE BOOL "" FORCE)

#if(_HAB_FULL_BUILD)
  ########################
  #### Super-Build #######
  ########################

  # Builds absl project from the git submodule.
  ExternalProject_Add(absl
    PREFIX absl
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/grpc/third_party/abseil-cpp"
    CMAKE_CACHE_ARGS
          -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/absl
  )

  # Builds c-ares project from the git submodule.
  ExternalProject_Add(c-ares
    PREFIX c-ares
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/grpc/third_party/cares/cares"
    CMAKE_CACHE_ARGS
          -DCARES_SHARED:BOOL=OFF
          -DCARES_STATIC:BOOL=ON
          -DCARES_STATIC_PIC:BOOL=ON
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares
  )

  # Builds protobuf project from the git submodule.
  ExternalProject_Add(protobuf
    PREFIX protobuf
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/grpc/third_party/protobuf/cmake"
    CMAKE_CACHE_ARGS
          -Dprotobuf_BUILD_TESTS:BOOL=OFF
          -Dprotobuf_WITH_ZLIB:BOOL=OFF
          -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf
  )

  # Builds re2 project from the git submodule.
  ExternalProject_Add(re2
    PREFIX re2
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/grpc/third_party/re2"
    CMAKE_CACHE_ARGS
          -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=TRUE
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/re2
  )

  # Builds zlib project from the git submodule.
  ExternalProject_Add(zlib
    PREFIX zlib
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/grpc/third_party/zlib"
    CMAKE_CACHE_ARGS
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/zlib
  )

  # the location where protobuf-config.cmake will be installed varies by platform
  if (WIN32)
    set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/cmake")
  else()
    set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib/cmake/protobuf")
  endif()

  # if OPENSSL_ROOT_DIR is set, propagate that hint path to the external projects with OpenSSL dependency.
  set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "")
  if (OPENSSL_ROOT_DIR)
    set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "-DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}")
  endif()

  # Builds gRPC based on locally checked-out sources and set arguments so that all the dependencies
  # are correctly located.
  ExternalProject_Add(grpc
    PREFIX grpc
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/grpc"
    CMAKE_CACHE_ARGS
          -DgRPC_INSTALL:BOOL=ON
          -DgRPC_BUILD_TESTS:BOOL=OFF
          -DgRPC_BUILD_MSVC_MP_COUNT:STRING=-1
          -DgRPC_PROTOBUF_PROVIDER:STRING=package
          -DgRPC_PROTOBUF_PACKAGE_TYPE:STRING=CONFIG
          -DProtobuf_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib/cmake/protobuf
          -DgRPC_RE2_PROVIDER:STRING=package
          -Dre2_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/re2/lib/cmake/re2
          -DgRPC_ZLIB_PROVIDER:STRING=package
          -DZLIB_ROOT:STRING=${CMAKE_CURRENT_BINARY_DIR}/zlib
          -DgRPC_ABSL_PROVIDER:STRING=package
          -Dabsl_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/absl/lib/cmake/absl
          -DgRPC_CARES_PROVIDER:STRING=package
          -Dc-ares_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares/lib/cmake/c-ares
          -DgRPC_SSL_PROVIDER:STRING=package
          ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/grpc
    DEPENDS c-ares protobuf re2 zlib absl
  )

  ExternalProject_Add(glfw
    PREFIX glfw
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw"
    CMAKE_CACHE_ARGS
          -DCMAKE_CXX_FLAGS:STRING=pkg-config --libs glfw3
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/glfw
          -DGLFW_BUILD_DOCS:BOOL=OFF
          -DGLFW_BUILD_TESTS:BOOL=OFF 
          -DGLFW_BUILD_EXAMPLES:BOOL=OFF
          -DGLFW_INSTALL:BOOL=ON
  )

  #ExternalProject_Add(Habitify
  #  PREFIX Habitify
  #  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Habitify"
  #  INSTALL_COMMAND ""
  #  CMAKE_CACHE_ARGS
  #        -D_HAB_CREATE_PROTOFILES:BOOL=_HAB_CREATE_PROTOFILES_var
  #        -Dglfw3_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/glfw/lib/cmake/glfw3
  #        -DProtobuf_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib/cmake/protobuf
  #        -Dc-ares_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares/lib/cmake/c-ares
  #        -Dre2_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/re2/lib/cmake/re2
  #        -DZLIB_ROOT:STRING=${CMAKE_CURRENT_BINARY_DIR}/zlib
  #        -Dabsl_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/absl/lib/cmake/absl
  #        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
  #        -DgRPC_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/grpc/lib/cmake/grpc
  #        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/Habitify
  #  DEPENDS protobuf grpc glfw
  #)
#
  #ExternalProject_Add(HabitifyServer
  #  PREFIX HabitifyServer
  #  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/HabitifyServer"
  #  INSTALL_COMMAND ""
  #  DEPENDS grpc Habitify
  #)

#else()
#  #####################
#  ### Partial Build ###
#  #####################
#    if(_HAB_APP_BUILD)
#      ExternalProject_Add(Habitify
#      PREFIX Habitify
#      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Habitify"
#      INSTALL_COMMAND ""
#      CMAKE_CACHE_ARGS
#            -D_HAB_CREATE_PROTOFILES:BOOL=_HAB_CREATE_PROTOFILES_var
#            -Dglfw3_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/glfw/lib/cmake/glfw3
#            -DProtobuf_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib/cmake/protobuf
#            -Dc-ares_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares/lib/cmake/c-ares
#            -Dre2_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/re2/lib/cmake/re2
#            -DZLIB_ROOT:STRING=${CMAKE_CURRENT_BINARY_DIR}/zlib
#            -Dabsl_DIR:STRING=${CMAKE_CURRENT_BINARY_DIR}/absl/lib/cmake/absl
#            ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
#            -DgRPC_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/grpc/lib/cmake/grpc
#      )
#    endif()
#
#    if(_HAB_SERVER_BUILD)
#      ExternalProject_Add(HabitifyServer
#      PREFIX HabitifyServer
#      INSTALL_COMMAND ""
#      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/HabitifyServer"
#      )
#    endif()
#
#endif()

