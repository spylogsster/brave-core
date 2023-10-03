// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/components/psst/browser/core/psst_rule.h"

#include <memory>
#include <utility>
#include <vector>

#include "base/files/file_path.h"
#include "base/json/json_reader.h"
#include "base/types/expected.h"
#include "extensions/common/url_pattern.h"
#include "net/base/url_util.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"
#include "url/origin.h"
#include "url/url_constants.h"

namespace {

// Format:
// [
//   {
//     "include": [
//       "*://twitter.com"
//     ],
//     "exclude": [
//     ],
//     "version": 1,
//     "test_script": "twitter/test.js",
//     "policy_script": "twitter/policy.js"
//   }, ...
// ]
// Note that "test_script" and "policy_script" give paths
// relative to the component under scripts/

// psst.json keys
const char kInclude[] = "include";
const char kExclude[] = "exclude";
const char kTestScript[] = "test_script";
const char kPolicyScript[] = "policy_script";
const char kVersion[] = "version";

}  // namespace

namespace psst {

PsstRule::PsstRule() = default;

PsstRule::~PsstRule() = default;

// static
bool PsstRule::GetURLPatternSetFromValue(const base::Value* value,
                                         extensions::URLPatternSet* result) {
  if (!value->is_list()) {
    return false;
  }
  std::string error;
  bool valid = result->Populate(
      value->GetList(), URLPattern::SCHEME_HTTP | URLPattern::SCHEME_HTTPS,
      false, &error);
  if (!valid) {
    DVLOG(1) << error;
  }
  return valid;
}

// static
bool PsstRule::GetFilePathFromValue(const base::Value* value,
                                    base::FilePath* result) {
  if (!value->is_string()) {
    return false;
  }
  *result = base::FilePath(FILE_PATH_LITERAL(value->GetString()));
  return true;
}

// static
void PsstRule::RegisterJSONConverter(
    base::JSONValueConverter<PsstRule>* converter) {
  converter->RegisterCustomValueField<extensions::URLPatternSet>(
      kInclude, &PsstRule::include_pattern_set_, GetURLPatternSetFromValue);
  converter->RegisterCustomValueField<extensions::URLPatternSet>(
      kExclude, &PsstRule::exclude_pattern_set_, GetURLPatternSetFromValue);
  converter->RegisterCustomValueField<base::FilePath>(
      kTestScript, &PsstRule::test_script_path_, GetFilePathFromValue);
  converter->RegisterCustomValueField<base::FilePath>(
      kPolicyScript, &PsstRule::policy_script_path_, GetFilePathFromValue);
  converter->RegisterIntField(kVersion, &PsstRule::version_);
}

// static
absl::optional<std::vector<std::unique_ptr<PsstRule>>> PsstRule::ParseRules(
    const std::string& contents) {
  if (contents.empty()) {
    return absl::nullopt;
  }
  absl::optional<base::Value> root = base::JSONReader::Read(contents);
  if (!root) {
    return absl::nullopt;
  }
  std::vector<std::unique_ptr<PsstRule>> rules;
  base::JSONValueConverter<PsstRule> converter;
  for (base::Value& it : root->GetList()) {
    std::unique_ptr<PsstRule> rule = std::make_unique<PsstRule>();
    if (!converter.Convert(it, rule.get())) {
      continue;
    }
    rules.push_back(std::move(rule));
  }
  return std::vector<std::unique_ptr<PsstRule>>(std::move(rules));
}

bool PsstRule::ShouldInsertScript(const GURL& url) const {
  // If URL matches an explicitly excluded pattern, this rule does not
  // apply.
  if (exclude_pattern_set_.MatchesURL(url)) {
    return false;
  }
  // If URL does not match an explicitly included pattern, this rule does not
  // apply.
  if (!include_pattern_set_.MatchesURL(url)) {
    return false;
  }

  return true;
}

}  // namespace psst
