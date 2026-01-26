//
// Created by bobok on 23/01/2026.
//

#include "StorageCore.h"

#include <stdexcept>

namespace storage {
    [[nodiscard]] std::optional<std::string> StorageCore::get(const std::string_view key) {
        if (const auto it = store.find(std::string(key)); it != store.end()) {
            return it->second;
        }

        return std::nullopt;
    }

    bool StorageCore::set(const std::string_view key, const std::string_view value) {
        store[std::string(key)] = std::string(value);
        return true;
    }

    bool StorageCore::remove(const std::string_view key) {
        return store.erase(std::string(key)) > 0;
    }
} // storage