// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/components/psst/core/psst_rule_service.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/contains.h"
#include "base/containers/flat_set.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/functional/callback_forward.h"
#include "base/logging.h"
#include "base/memory/singleton.h"
#include "base/task/thread_pool.h"
#include "brave/components/psst/common/features.h"
#include "brave/components/psst/core/psst_rule.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace psst {

constexpr char kJsonFile[] = "psst.json";
constexpr char kScriptsDir[] = "scripts";

// static
PsstRuleService* PsstRuleService::GetInstance() {
  // Check if feature flag is enabled.
  if (!base::FeatureList::IsEnabled(psst::features::kBravePsst)) {
    return nullptr;
  }
  return base::Singleton<PsstRuleService>::get();
}

PsstRuleService::PsstRuleService() = default;

PsstRuleService::~PsstRuleService() = default;

std::string ReadFile(const base::FilePath& file_path) {
  std::string contents;
  bool success = base::ReadFileToString(file_path, &contents);
  if (!success || contents.empty()) {
    VLOG(2) << "ReadFile: cannot "
            << "read file " << file_path;
  }
  return contents;
}

MatchedRule CreateMatchedRule(const base::FilePath& component_path,
                              const base::FilePath& test_script_path,
                              const base::FilePath& policy_script_path,
                              const int version) {
  auto prefix =
      base::FilePath(component_path).Append(FILE_PATH_LITERAL(kScriptsDir));
  auto test_script = ReadFile(base::FilePath(prefix).Append(test_script_path));
  auto policy_script =
      ReadFile(base::FilePath(prefix).Append(policy_script_path));
  return {test_script, policy_script, version};
}

void PsstRuleService::CheckIfMatch(
    const GURL& url,
    base::OnceCallback<void(MatchedRule)> cb) const {
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

void PsstRuleService::LoadPsstRules(const base::FilePath& path) {
  component_path_ = path;
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::BindOnce(&ReadFile, path.AppendASCII(kJsonFile)),
      base::BindOnce(&PsstRuleService::OnFileDataReady,
                     weak_factory_.GetWeakPtr()));
}

void PsstRuleService::OnFileDataReady(const std::string& contents) {
  auto parsed_rules = PsstRule::ParseRules(contents);
  if (!parsed_rules) {
    VLOG(1) << "OnFileDataReady: cannot parse rules";
    return;
  }
  rules_.clear();
  rules_ = std::move(parsed_rules.value());
}

}  // namespace psst
