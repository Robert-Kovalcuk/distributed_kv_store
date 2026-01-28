//
// Created by bobok on 28/01/2026.
//

#include <grpcpp/create_channel.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/credentials.h>
#include <gtest/gtest.h>

#include "kv.grpc.pb.h"
#include "kvstore/StorageCore.h"
#include "kvstore/SynchronizedStorage.h"
#include "server/ServerService.h"

class KVClient {
    std::unique_ptr<kv::KV::Stub> stub_;

public:
    explicit KVClient(const std::shared_ptr<grpc::Channel> &channel)
        : stub_(kv::KV::NewStub(channel)) {
    }

    [[nodiscard]] bool set(const std::string_view key, const std::string_view value) const {
        grpc::ClientContext context;
        kv::PutRequest req;
        req.set_key(std::string(key));
        req.set_value(std::string(value));

        kv::PutReply rep;
        const auto status = stub_->Put(&context, req, &rep);
        return status.ok();
    }

    [[nodiscard]] bool remove(const std::string_view key) const {
        grpc::ClientContext context;
        kv::DeleteRequest req;
        req.set_key(std::string(key));

        kv::DeleteReply rep;
        const auto status = stub_->Delete(&context, req, &rep);
        return status.ok();
    }

    [[nodiscard]] std::optional<std::string> get(const std::string_view key) const {
        grpc::ClientContext context;
        kv::GetRequest req;
        req.set_key(std::string(key));

        kv::GetReply rep;
        if (const auto status = stub_->Get(&context, req, &rep); status.ok() && rep.found()) return rep.value();
        return std::nullopt;
    }
};

class KVClientFixture : public ::testing::Test {
protected:
    std::shared_ptr<grpc::Channel> m_channel_;
    storage::StorageCore m_storage_core_;
    std::shared_ptr<storage::SynchronizedStorage> m_storage_;
    ServerService m_server_service_;
    grpc::ServerBuilder m_server_builder_;
    std::unique_ptr<grpc::Server> m_server_;
    std::unique_ptr<KVClient> m_client_;

public:
    KVClientFixture()
        : m_storage_(std::make_shared<storage::SynchronizedStorage>(m_storage_core_)), m_server_service_(m_storage_) {
        m_channel_ = grpc::CreateChannel("127.0.0.1", grpc::InsecureChannelCredentials());
    };

protected:
    void SetUp() override {
        m_server_builder_.RegisterService(&m_server_service_);
        m_server_builder_.AddListeningPort("127.0.0.1", grpc::InsecureServerCredentials());
        m_server_ = m_server_builder_.BuildAndStart();
        m_client_ = std::make_unique<KVClient>(m_channel_);

        ASSERT_NE(m_server_, nullptr);

        // const auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(2);
        // ASSERT_TRUE(m_channel_->WaitForConnected(deadline));
    }

    void TearDown() override {
        if (m_server_) {
            m_server_->Shutdown();
            m_server_->Wait();
        }
    }
};

TEST_F(KVClientFixture, PutThenGet_Works) {
    ASSERT_TRUE(m_client_->set("k1", "v1"));

    const auto out = m_client_->get("k1");
    ASSERT_TRUE(out.has_value());
    EXPECT_EQ(*out, "v1");
}

TEST_F(KVClientFixture, GetMissing_ReturnsEmpty) {
    const auto out = m_client_->get("missing");
    EXPECT_FALSE(out.has_value());
}

TEST_F(KVClientFixture, DeleteThenGet_ReturnsEmpty) {
    ASSERT_TRUE(m_client_->set("k2", "v2"));
    ASSERT_TRUE(m_client_->remove("k2"));

    const auto out = m_client_->get("k2");
    EXPECT_FALSE(out.has_value());
}