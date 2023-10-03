// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_PSST_CORE_PSST_RULE_H_
#define BRAVE_COMPONENTS_PSST_CORE_PSST_RULE_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/flat_set.h"
#include "base/files/file_path.h"
#include "base/json/json_value_converter.h"
#include "base/values.h"
#include "extensions/common/url_pattern_set.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class GURL;

namespace psst {

struct MatchedRule {
  std::string test_script;
  std::string policy_script;
  int version;
};

class PsstRule {
 public:
  PsstRule();
  ~PsstRule();

  // Registers the mapping between JSON field names and the members in this
  // class.
  static void RegisterJSONConverter(
      base::JSONValueConverter<PsstRule>* converter);
  static absl::optional<std::vector<std::unique_ptr<PsstRule>>> ParseRules(
      const std::string& contents);
  static bool GetURLPatternSetFromValue(const base::Value* value,
                                        extensions::URLPatternSet* result);
  static bool GetFilePathFromValue(const base::Value* value,
                                   base::FilePath* result);
  bool ShouldInsertScript(const GURL& url) const;
  const extensions::URLPatternSet& include_pattern_set() const {
    return include_pattern_set_;
  }

  const base::FilePath& GetPolicyScriptPath() const {
    return policy_script_path_;
  }

  const base::FilePath& GetTestScriptPath() const { return test_script_path_; }

  int GetVersion() const { return version_; }

 private:
  extensions::URLPatternSet include_pattern_set_;
  extensions::URLPatternSet exclude_pattern_set_;
  base::FilePath policy_script_path_;
  base::FilePath test_script_path_;
  int version_;
};

}  // namespace psst

#endif  // BRAVE_COMPONENTS_PSST_CORE_PSST_RULE_H_
