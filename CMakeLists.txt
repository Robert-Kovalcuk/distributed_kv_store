cmake_minimum_required(VERSION 3.24)
project(DistributedKV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

# =======================
# PROTO / gRPC CODEGEN
# =======================
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(PROTO_FILE ${PROTO_DIR}/kv.proto)

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GEN_DIR})

get_target_property(PROTOC_EXE protobuf::protoc LOCATION)
get_target_property(GRPC_CPP_PLUGIN_EXE gRPC::grpc_cpp_plugin LOCATION)

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
target_include_directories(kv_proto PUBLIC "${GEN_DIR}")
target_link_libraries(kv_proto PUBLIC gRPC::grpc++ protobuf::libprotobuf)

# =======================
# EXECUTABLES
# =======================
add_executable(DistributedKV src/main.cpp
        src/kvstore/SynchronizedStorage.cpp
        src/kvstore/SynchronizedStorage.h
        src/kvstore/StorageCore.cpp
        src/kvstore/StorageCore.h
        src/kvstore/IStorage.h
)
target_link_libraries(DistributedKV PRIVATE kv_proto)

add_executable(kv_client src/client/client.cpp)
target_link_libraries(kv_client PRIVATE kv_proto)

if (MSVC)
    target_compile_options(kv_client PRIVATE /W4 /permissive-)
    target_compile_options(DistributedKV PRIVATE /W4 /permissive-)
endif()
