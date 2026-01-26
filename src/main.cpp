#include <grpcpp/grpcpp.h>
#include "kv.grpc.pb.h"

#include <iostream>
#include <memory>
#include <string>

#include "kvstore/SynchronizedStorage.h"
#include "server/ServerService.h"


int main() {
    const std::string server_address = "127.0.0.1";

    storage::StorageCore storage_core{};
    const auto sync_storage = std::make_shared<storage::SynchronizedStorage>(storage_core);

    ServerService service{sync_storage};

    grpc::ServerBuilder server_builder;
    server_builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    server_builder.RegisterService(&service);

    const std::unique_ptr server(server_builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

    return 0;
}
