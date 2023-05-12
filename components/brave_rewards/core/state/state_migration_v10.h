/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_REWARDS_CORE_STATE_STATE_MIGRATION_V10_H_
#define BRAVE_COMPONENTS_BRAVE_REWARDS_CORE_STATE_STATE_MIGRATION_V10_H_

#include <memory>
#include <string>

#include "brave/components/brave_rewards/core/endpoint/promotion/get_wallet/get_wallet.h"
#include "brave/components/brave_rewards/core/ledger_callbacks.h"

namespace brave_rewards::internal::state {

class StateMigrationV10 {
 public:
  StateMigrationV10();
  ~StateMigrationV10();

  void Migrate(LegacyResultCallback callback);

 private:
  void OnGetWallet(mojom::Result result,
                   const std::string& custodian,
                   bool linked,
                   LegacyResultCallback callback);

  std::unique_ptr<endpoint::promotion::GetWallet> get_wallet_;
};

}  // namespace brave_rewards::internal

#endif  // BRAVE_COMPONENTS_BRAVE_REWARDS_CORE_STATE_STATE_MIGRATION_V10_H_
