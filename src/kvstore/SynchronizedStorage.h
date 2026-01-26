//
// Created by bobok on 23/01/2026.
//

#ifndef DISTRIBUTEDKV_SYNCHRONIZEDSTORAGE_H
#define DISTRIBUTEDKV_SYNCHRONIZEDSTORAGE_H
#include <mutex>
#include <optional>

#include "StorageCore.h"

/**
 *
 */
namespace storage {
    using std::string;
    using std::string_view;

    class SynchronizedStorage final {
        IStorage& m_storage;
        std::mutex m_storage_mutex;

    public:
        explicit SynchronizedStorage(IStorage &m_storage)
            : m_storage(m_storage) {
        }

        [[nodiscard]] std::optional<string> get(string_view key) ;
        bool set(string_view key, string_view value) ;
        bool remove(string_view key) ;
    };
} // storage

#endif //DISTRIBUTEDKV_SYNCHRONIZEDSTORAGE_H