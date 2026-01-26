//
// Created by bobok on 26/01/2026.
//

#ifndef DISTRIBUTEDKV_SERVERSERVICE_H
#define DISTRIBUTEDKV_SERVERSERVICE_H

#include "kv.grpc.pb.h"
#include "kv.pb.h"
#include "../kvstore/IStorage.h"

class ServerService final : public kv::KV::Service {
    const std::shared_ptr<IStorage> m_storage;

public:
    explicit ServerService(const std::shared_ptr<IStorage> &m_storage)
        : m_storage(m_storage) {
    }

    grpc::Status Put(grpc::ServerContext *context, const kv::PutRequest *request, kv::PutReply *response) override;
    grpc::Status Get(grpc::ServerContext *context, const kv::GetRequest *request, kv::GetReply *response) override;
    grpc::Status Delete(grpc::ServerContext *context, const kv::DeleteRequest *request,
        kv::DeleteReply *response) override;
    grpc::Status List(grpc::ServerContext *context, const kv::ListRequest *request, kv::ListReply *response) override;
};


#endif //DISTRIBUTEDKV_SERVERSERVICE_H