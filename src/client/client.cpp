#include <grpcpp/grpcpp.h>
#include "kv.grpc.pb.h"

#include <iostream>
#include <memory>
#include <string>

class KVClientFixture {
    std::unique_ptr<kv::KV::Stub> m_stub_;
public:
    explicit KVClientFixture(const std::shared_ptr<grpc::Channel>& channel) : m_stub_(kv::KV::NewStub(channel)) { }

    [[nodiscard]] bool set(std::string_view key, std::string_view value) const {
        grpc::ClientContext context;

        auto request = kv::PutRequest{};
        request.set_key(key);
        request.set_value(value);

        auto response = kv::PutReply{};

        const auto status = m_stub_->Put(&context, request, &response);

        return status.ok();
    }

    [[nodiscard]] bool remove(std::string_view key) const {
        grpc::ClientContext context;

        auto request = kv::DeleteRequest{};

        request.set_key(key);

        auto response = kv::DeleteReply{};

        const auto status = m_stub_->Delete(&context, request, &response);

        return status.ok();
    }

    [[nodiscard]] std::optional<std::string> get(std::string_view key) const {
        grpc::ClientContext context;

        auto request = kv::GetRequest{};
        request.set_key(key);

        auto response = kv::GetReply{};

        if (const auto status = m_stub_->Get(&context, request, &response); status.ok() && response.found()) {
            return response.value();
        }

        return {};
    }
};

int main() {
    const auto channel = grpc::CreateChannel("127.0.0.1", grpc::InsecureChannelCredentials());

    const auto client = KVClientFixture{channel};

    auto result = client.set("1", "1");
    result = client.set("2", "2");



    std::cout << client.set("woho", "moho") << std::endl;
}
