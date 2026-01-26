//
// Created by bobok on 26/01/2026.
//

#include "ServerService.h"

grpc::Status ServerService::Put(grpc::ServerContext *context, const kv::PutRequest *request, kv::PutReply *response) {
    if (const auto result = m_storage->set(request->key(), request->value()); !result) {
        return grpc::Status::CANCELLED;
    }

    return grpc::Status::OK;
}

grpc::Status ServerService::Get(grpc::ServerContext *context, const kv::GetRequest *request, kv::GetReply *response) {
    const auto result = m_storage->get(request->key());

    if (!result) {
        response->set_found(false);
        return grpc::Status::CANCELLED;
    }

    response->set_found(true);
    response->set_value(result.value());

    return grpc::Status::OK;
}

grpc::Status ServerService::Delete(grpc::ServerContext *context, const kv::DeleteRequest *request,
    kv::DeleteReply *response) {

    if (const auto result = m_storage->remove(request->key()); !result) {
        response->set_removed(false);
        return grpc::Status::CANCELLED;
    }

    response->set_removed(true);
    return grpc::Status::OK;
}

grpc::Status ServerService::List(grpc::ServerContext *context, const kv::ListRequest *request,
    kv::ListReply *response) {
    return Service::List(context, request, response);
}
