cmake_minimum_required(VERSION 3.24)
project(DistributedKV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =======================
# PACKAGES
# =======================
find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# =======================
# CTEST
# =======================
include(CTest)
enable_testing()

# =======================
# PROTO / gRPC CODEGEN
# =======================
set(PROTO_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(PROTO_FILE ${PROTO_DIR}/kv.proto)

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GEN_DIR})

# More robust than LOCATION:
set(PROTOC_EXE          $<TARGET_FILE:protobuf::protoc>)
set(GRPC_CPP_PLUGIN_EXE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

set(GEN_SRCS
        ${GEN_DIR}/kv.pb.cc
        ${GEN_DIR}/kv.grpc.pb.cc
)
set(GEN_HDRS
        ${GEN_DIR}/kv.pb.h
        ${GEN_DIR}/kv.grpc.pb.h
)

add_custom_command(
        OUTPUT ${GEN_SRCS} ${GEN_HDRS}
        COMMAND ${PROTOC_EXE}
        ARGS
        --proto_path=${PROTO_DIR}
        --cpp_out=${GEN_DIR}
        --grpc_out=${GEN_DIR}
        --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXE}
        ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
        VERBATIM
)

add_library(kv_proto ${GEN_SRCS} ${GEN_HDRS})
target_include_directories(kv_proto PUBLIC ${GEN_DIR})
target_link_libraries(kv_proto PUBLIC gRPC::grpc++ protobuf::libprotobuf)

# =======================
# CORE LIB (PROD CODE)
# =======================
add_library(kv_core
        src/kvstore/SynchronizedStorage.cpp
        src/kvstore/StorageCore.cpp
        src/server/ServerService.cpp
)

# So tests/exes can do: #include "kvstore/StorageCore.h"
target_include_directories(kv_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(kv_core PUBLIC kv_proto)

# =======================
# EXECUTABLES
# =======================
add_executable(DistributedKV
        src/main.cpp
)
target_link_libraries(DistributedKV PRIVATE kv_core)

add_executable(kv_client
        src/client/client.cpp
)
target_link_libraries(kv_client PRIVATE kv_proto)

# =======================
# TESTS
# =======================
add_executable(kv_tests
        test/storage_unit_test.cpp
        test/grpc_integration_test.cpp
)
target_link_libraries(kv_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        kv_core
)

add_test(NAME kv_tests COMMAND kv_tests)

# =======================
# WARNINGS (MSVC)
# =======================
if (MSVC)
    target_compile_options(kv_client      PRIVATE /W4 /permissive-)
    target_compile_options(DistributedKV PRIVATE /W4 /permissive-)
    target_compile_options(kv_tests      PRIVATE /W4 /permissive-)
    target_compile_options(kv_core       PRIVATE /W4 /permissive-)
endif()
