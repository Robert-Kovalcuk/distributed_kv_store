//
// Created by bobok on 23/01/2026.
//

#ifndef DISTRIBUTEDKV_ISTORAGE_H
#define DISTRIBUTEDKV_ISTORAGE_H
#include <optional>
#include <string>

struct IStorage {
    virtual ~IStorage() = default;

    virtual std::optional<std::string>  get(std::string_view key);
    virtual bool set(std::string_view key, std::string_view value);
    virtual bool remove(std::string_view key);
};

#endif //DISTRIBUTEDKV_ISTORAGE_H