#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <memory>

#include "aws_api_handle.h"
#include "gtest/gtest_pred_impl.h"
#include "not_null.hpp"

namespace {
TEST(ApiHandle, InstanceSharing) {
  auto* old_instance = (stonks::aws::ApiHandle*){};

  {
    auto instance1 = stonks::aws::ApiHandle::Instance();
    auto instance2 = stonks::aws::ApiHandle::Instance();
    old_instance = instance1.get();

    EXPECT_EQ(instance1.get(), instance2.get());
    EXPECT_EQ(instance1.get(), old_instance);
  }

  auto instance3 = stonks::aws::ApiHandle::Instance();
  EXPECT_NE(instance3.get(), old_instance);
}
}  // namespace