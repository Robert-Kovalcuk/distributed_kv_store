//
// Created by bobok on 23/01/2026.
//

#ifndef DISTRIBUTEDKV_ISTORAGE_H
#define DISTRIBUTEDKV_ISTORAGE_H
#include <optional>
#include <string>
#include <string_view>

struct IStorage {
    virtual ~IStorage() = default;

    virtual std::optional<std::string>  get(std::string_view key) = 0;
    virtual bool set(std::string_view key, std::string_view value) = 0;
    virtual bool remove(std::string_view key) = 0;
};

#endif //DISTRIBUTEDKV_ISTORAGE_H