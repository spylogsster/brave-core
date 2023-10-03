// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "brave/components/psst/browser/content/psst_tab_helper.h"

#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/utf_string_conversions.h"
#include "base/task/thread_pool.h"
#include "base/values.h"
#include "brave/components/psst/browser/core/psst_rule.h"
#include "brave/components/psst/browser/core/psst_rule_service.h"
#include "components/sessions/content/session_tab_helper.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/web_contents.h"

namespace psst {

PsstTabHelper::PsstTabHelper(content::WebContents* web_contents,
                             const int32_t world_id)
    : WebContentsObserver(web_contents),
      content::WebContentsUserData<PsstTabHelper>(*web_contents),
      world_id_(world_id),
      weak_factory_(this) {
  psst_service_ = PsstRuleService::GetInstance();
  if (!psst_service_) {
    return;
  }
}

PsstTabHelper::~PsstTabHelper() = default;

void InsertScriptInPage(
    const content::GlobalRenderFrameHostId render_frame_host_id,
    const int32_t world_id,
    const std::string& script,
    content::RenderFrameHost::JavaScriptResultCallback cb) {
  content::RenderFrameHost* render_frame_host =
      content::RenderFrameHost::FromID(render_frame_host_id);

  if (render_frame_host) {
    render_frame_host->ExecuteJavaScriptInIsolatedWorld(
        base::UTF8ToUTF16(script), std::move(cb), world_id);
  }
}

void InsertPolicyScript(
    const content::GlobalRenderFrameHostId render_frame_host_id,
    const int32_t world_id,
    std::string policy_script) {
  InsertScriptInPage(render_frame_host_id, world_id, policy_script,
                     base::DoNothing());
}

void OnTestScriptResult(
    std::string policy_script,
    const content::GlobalRenderFrameHostId render_frame_host_id,
    const int32_t world_id,
    base::Value value) {
  if (value.GetIfBool().value_or(false)) {
    InsertPolicyScript(render_frame_host_id, world_id, policy_script);
  }
}

void InsertTestScript(
    const content::GlobalRenderFrameHostId render_frame_host_id,
    const int32_t world_id,
    MatchedRule rule) {
  CHECK(render_frame_host_id);

  InsertScriptInPage(render_frame_host_id, world_id, rule.test_script,
                     base::BindOnce(&OnTestScriptResult, rule.policy_script,
                                    render_frame_host_id, world_id));
}

void PsstTabHelper::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  CHECK(navigation_handle);

  if (!psst_service_ || !navigation_handle->IsInPrimaryMainFrame() ||
      !navigation_handle->HasCommitted()) {
    return;
  }

  const bool tab_not_restored =
      navigation_handle->GetRestoreType() == content::RestoreType::kNotRestored;

  if (!navigation_handle->IsSameDocument()) {
    should_process_ = tab_not_restored;
    return;
  }
}

void PsstTabHelper::DocumentOnLoadCompletedInPrimaryMainFrame() {
  auto url = web_contents()->GetLastCommittedURL();
  if (!psst_service_ || !should_process_) {
    return;
  }

  content::GlobalRenderFrameHostId render_frame_host_id =
      web_contents()->GetPrimaryMainFrame()->GetGlobalId();

  psst_service_->CheckIfMatch(
      url, base::BindOnce(&InsertTestScript, render_frame_host_id, world_id_));
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(PsstTabHelper);

}  // namespace psst
