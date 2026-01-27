#include <gtest/gtest.h>
#include "kvstore/SynchronizedStorage.h"

class BasicsFixture : public ::testing::Test {
protected:
    storage::StorageCore core;
    storage::SynchronizedStorage storage{core};
};

// --------------------
// get / set / remove
// --------------------

TEST_F(BasicsFixture, SetThenGet_ReturnsValue) {
    const char* key = "key";
    const char* value_in = "value";

    ASSERT_TRUE(storage.set(key, value_in));

    const auto value_out = storage.get(key);
    ASSERT_TRUE(value_out.has_value());
    EXPECT_EQ(*value_out, value_in);
}

TEST_F(BasicsFixture, GetMissing_ReturnsEmptyOptional) {
    const char* missing = "missing";

    const auto value_out = storage.get(missing);
    EXPECT_FALSE(value_out.has_value());
}

TEST_F(BasicsFixture, RemoveExisting_ReturnsTrue_AndThenMissing) {
    const char* key = "key";
    const char* value_in = "value";

    ASSERT_TRUE(storage.set(key, value_in));

    EXPECT_TRUE(storage.remove(key));

    const auto value_out = storage.get(key);
    EXPECT_FALSE(value_out.has_value());
}

TEST_F(BasicsFixture, RemoveMissing_ReturnsFalse) {
    const char* missing = "missing";
    EXPECT_FALSE(storage.remove(missing));
}

TEST_F(BasicsFixture, SetOverwrite_LastWriteWins) {
    const char* key = "key";

    ASSERT_TRUE(storage.set(key, "v1"));
    ASSERT_TRUE(storage.set(key, "v2"));

    const auto value_out = storage.get(key);
    ASSERT_TRUE(value_out.has_value());
    EXPECT_EQ(*value_out, "v2");
}

// --------------------
// invalid input policy
// --------------------
// This test encodes a specific policy:
// - empty key is invalid
// - whitespace-only key is invalid
// - empty value is allowed (common for KV stores)
// If your implementation differs, change the expectations accordingly.

TEST_F(BasicsFixture, InvalidKeys_AreRejected) {
    EXPECT_FALSE(storage.set("", "value"));
    EXPECT_FALSE(storage.set(" \n\t", "value"));

    // should not create entries
    EXPECT_FALSE(storage.get("").has_value());
    EXPECT_FALSE(storage.get(" \n\t").has_value());
    EXPECT_FALSE(storage.remove(""));
}

TEST_F(BasicsFixture, EmptyValue_IsAllowed) {
    const char* key = "k";

    ASSERT_TRUE(storage.set(key, ""));

    const auto value_out = storage.get(key);
    ASSERT_TRUE(value_out.has_value());
    EXPECT_EQ(*value_out, "");
}
