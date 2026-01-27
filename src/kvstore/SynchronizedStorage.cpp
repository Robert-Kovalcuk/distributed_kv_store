//
// Created by bobok on 23/01/2026.
//

#include "SynchronizedStorage.h"

#include <optional>
#include <algorithm>

namespace storage {
    std::optional<string> SynchronizedStorage::get(const string_view key)  {
        std::scoped_lock lock(m_storage_mutex);
        const auto value = m_storage.get(key);

        return value;
    }

    bool SynchronizedStorage::set(const string_view key, const string_view value)  {
        std::scoped_lock lock(m_storage_mutex);

        if (key.empty() || std::ranges::all_of(key, [](const unsigned char ch) {
            return std::isspace(ch);
        }) == true) {
            return false;
        }

        const auto success = m_storage.set(key, value);

        return success;
    }

    bool SynchronizedStorage::remove(const string_view key)  {
        std::scoped_lock lock(m_storage_mutex);
        const auto value = m_storage.remove(key);

        return value;
    }
} // storage