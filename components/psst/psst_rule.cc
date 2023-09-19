// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/components/psst/psst_rule.h"

#include <memory>
#include <utility>
#include <vector>

#include "base/json/json_reader.h"
#include "base/types/expected.h"
#include "components/prefs/pref_service.h"
#include "extensions/common/url_pattern.h"
#include "net/base/url_util.h"
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

// Removes trailing dot from |host_piece| if any.
// Copied from extensions/common/url_pattern.cc
base::StringPiece CanonicalizeHostForMatching(base::StringPiece host_piece) {
  if (base::EndsWith(host_piece, ".")) {
    host_piece.remove_suffix(1);
  }
  return host_piece;
}

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
  *result = base::FilePath(value->GetString());
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
const std::string PsstRule::GetETLDForPsst(const std::string& host) {
  base::StringPiece host_piece = CanonicalizeHostForMatching(host);
  return net::registry_controlled_domains::GetDomainAndRegistry(
      host_piece, net::registry_controlled_domains::PrivateRegistryFilter::
                      EXCLUDE_PRIVATE_REGISTRIES);
}

// static
base::expected<std::pair<std::vector<std::unique_ptr<PsstRule>>,
                         base::flat_set<std::string>>,
               std::string>
PsstRule::ParseRules(const std::string& contents) {
  if (contents.empty()) {
    return base::unexpected("Could not obtain psst configuration");
  }
  absl::optional<base::Value> root = base::JSONReader::Read(contents);
  if (!root) {
    return base::unexpected("Failed to parse psst configuration");
  }
  std::vector<std::string> hosts;
  std::vector<std::unique_ptr<PsstRule>> rules;
  base::JSONValueConverter<PsstRule> converter;
  for (base::Value& it : root->GetList()) {
    std::unique_ptr<PsstRule> rule = std::make_unique<PsstRule>();
    if (!converter.Convert(it, rule.get())) {
      continue;
    }
    for (const URLPattern& pattern : rule->include_pattern_set()) {
      if (!pattern.host().empty()) {
        const std::string etldp1 = PsstRule::GetETLDForPsst(pattern.host());
        if (!etldp1.empty()) {
          hosts.push_back(std::move(etldp1));
        }
      }
    }
    rules.push_back(std::move(rule));
  }
  return std::pair<std::vector<std::unique_ptr<PsstRule>>,
                   base::flat_set<std::string>>(std::move(rules), hosts);
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
