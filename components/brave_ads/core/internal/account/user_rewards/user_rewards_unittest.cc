/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/user_rewards/user_rewards.h"

#include <memory>

#include "brave/components/brave_ads/core/internal/account/tokens/token_generator_mock.h"
#include "brave/components/brave_ads/core/internal/account/user_rewards/user_rewards_delegate_mock.h"
#include "brave/components/brave_ads/core/internal/account/wallet/wallet_unittest_util.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- brave_unit_tests --filter=BraveAds

namespace brave_ads {

class BraveAdsUserRewardsTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    user_rewards_ = std::make_unique<UserRewards>(&token_generator_mock_,
                                                  GetWalletForTesting());
    user_rewards_->SetDelegate(&delegate_mock_);
  }

  TokenGeneratorMock token_generator_mock_;

  std::unique_ptr<UserRewards> user_rewards_;
  UserRewardsDelegateMock delegate_mock_;
};

TEST_F(BraveAdsUserRewardsTest, FetchIssuers) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BraveAdsUserRewardsTest, RefillConfirmationTokens) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BraveAdsUserRewardsTest, RedeemPaymentTokens) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BraveAdsUserRewardsTest, MigrateVerifiedRewardsUser) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BraveAdsUserRewardsTest, DoNotMigrateVerifiedRewardsUser) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

TEST_F(BraveAdsUserRewardsTest, CaptchaRequiredToRefillConfirmationTokens) {
  // Arrange

  // Act

  // Assert
  FAIL();
}

}  // namespace brave_ads
