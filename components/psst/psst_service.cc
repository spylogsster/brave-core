// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/components/psst/psst_service.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "base/containers/contains.h"
#include "base/containers/flat_set.h"
#include "base/files/file_path.h"
#include "base/functional/callback_forward.h"
#include "base/logging.h"
#include "base/memory/singleton.h"
#include "base/task/thread_pool.h"
#include "brave/components/brave_component_updater/browser/dat_file_util.h"
#include "brave/components/psst/psst_rule.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace psst {

constexpr char kJsonFile[] = "psst.json";
constexpr char kScriptsDir[] = "scripts";

using base::AutoLock;

struct PsstServiceSingletonTrait
    : public base::DefaultSingletonTraits<PsstService> {
  static PsstService* New() {
    PsstService* instance = new PsstService();
    return instance;
  }
};

// static
PsstService* PsstService::GetInstance() {
  return base::Singleton<PsstService, PsstServiceSingletonTrait>::get();
}

PsstService::PsstService() = default;

PsstService::~PsstService() {
  AutoLock lock(lock_);  // DCHECK fail if the lock is held.
}

MatchedRule CreateMatchedRule(const base::FilePath& component_path,
                              const base::FilePath& test_script_path,
                              const base::FilePath& policy_script_path,
                              const int version) {
  auto prefix = base::FilePath(component_path).Append(kScriptsDir);
  auto test_script = brave_component_updater::GetDATFileAsString(
      base::FilePath(prefix).Append(test_script_path));
  auto policy_script = brave_component_updater::GetDATFileAsString(
      base::FilePath(prefix).Append(policy_script_path));
  return {test_script, policy_script, version};
}

void PsstService::CheckIfMatch(const GURL& url,
                               base::OnceCallback<void(MatchedRule)> cb) const {
  // Check host cache to see if this URL needs to have any psst rules
  // applied.
  AutoLock lock(lock_);
  const std::string etldp1 = PsstRule::GetETLDForPsst(url.host());
  if (!base::Contains(host_cache_, etldp1)) {
    return;
  }

  for (const std::unique_ptr<PsstRule>& rule : rules_) {
    if (rule->ShouldInsertScript(url)) {
      base::ThreadPool::PostTaskAndReplyWithResult(
          FROM_HERE, {base::MayBlock()},
          base::BindOnce(&CreateMatchedRule, component_path_,
                         rule->GetTestScriptPath(), rule->GetPolicyScriptPath(),
                         rule->GetVersion()),
          std::move(cb));
    }
  }
  return;
}

void PsstService::LoadPsstRules(const base::FilePath& path) {
  component_path_ = path;
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::BindOnce(&brave_component_updater::GetDATFileAsString,
                     path.AppendASCII(kJsonFile)),
      base::BindOnce(&PsstService::OnFileDataReady,
                     weak_factory_.GetWeakPtr()));
}

void PsstService::OnFileDataReady(const std::string& contents) {
  AutoLock lock(lock_);
  auto parsed_rules = PsstRule::ParseRules(contents);
  if (!parsed_rules.has_value()) {
    LOG(WARNING) << parsed_rules.error();
    return;
  }
  rules_.clear();
  host_cache_.clear();
  rules_ = std::move(parsed_rules.value().first);
  host_cache_ = parsed_rules.value().second;
}

}  // namespace psst
