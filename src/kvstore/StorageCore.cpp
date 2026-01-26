//
// Created by bobok on 23/01/2026.
//

#include "StorageCore.h"

namespace storage {
    [[nodiscard]] std::optional<std::string> StorageCore::get(const std::string_view key) {
        return store.at(key.data());
    }

    bool StorageCore::set(const std::string_view key, const std::string_view value) {
        store[key.data()] = value;

        return true;
    }

    bool StorageCore::remove(const std::string_view key) {
        store.erase(key.data());
        return true;
    }
} // storage