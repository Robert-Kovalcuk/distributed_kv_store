//
// Created by bobok on 23/01/2026.
//

#ifndef DISTRIBUTEDKV_STORAGECORE_H
#define DISTRIBUTEDKV_STORAGECORE_H
#include <string>
#include <unordered_map>

#include "IStorage.h"

namespace storage {
    using key_t = std::string;
    using value_t = std::string;
    using key_view = std::string_view;
    using value_view = std::string_view;


    class StorageCore final : public IStorage {
        std::unordered_map<key_t, value_t> store;

    public:
        StorageCore() = default;

        [[nodiscard]] std::optional<value_t> get(key_view key) override;
        bool set(key_view key, value_view value) override;
        bool remove(key_view key) override;
    };
} // storage

#endif //DISTRIBUTEDKV_STORAGECORE_H