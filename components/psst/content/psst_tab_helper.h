// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_PSST_CONTENT_PSST_TAB_HELPER_H_
#define BRAVE_COMPONENTS_PSST_CONTENT_PSST_TAB_HELPER_H_

#include "base/component_export.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"

#include "brave/components/psst/core/psst_rule.h"
#include "build/build_config.h"
#include "components/sessions/core/session_id.h"
#include "content/public/browser/media_player_id.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace psst {

class PsstService;

class COMPONENT_EXPORT(PSST_CONTENT) PsstTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<PsstTabHelper> {
 public:
  ~PsstTabHelper() override;
  PsstTabHelper(const PsstTabHelper&) = delete;
  PsstTabHelper& operator=(const PsstTabHelper&) = delete;

 private:
  COMPONENT_EXPORT(PSST_CONTENT)
  PsstTabHelper(content::WebContents*, const int32_t world_id);

  friend class content::WebContentsUserData<PsstTabHelper>;

  // content::WebContentsObserver overrides
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;
  void DocumentOnLoadCompletedInPrimaryMainFrame() override;

  const int32_t world_id_;
  raw_ptr<PsstService> psst_service_ = nullptr;  // NOT OWNED
  bool should_process_ = false;

  base::WeakPtrFactory<PsstTabHelper> weak_factory_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace psst

#endif  // BRAVE_COMPONENTS_PSST_CONTENT_PSST_TAB_HELPER_H_
