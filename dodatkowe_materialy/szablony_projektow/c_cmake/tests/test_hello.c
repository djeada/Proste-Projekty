#include <gtest/gtest.h>
#include "hello.h"

TEST(HelloTest, BasicAssertions) {
    EXPECT_STREQ(get_hello_message(), "Hello, World!");
}
