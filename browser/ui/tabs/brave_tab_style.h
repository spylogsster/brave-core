/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_TABS_BRAVE_TAB_STYLE_H_
#define BRAVE_BROWSER_UI_TABS_BRAVE_TAB_STYLE_H_

#include "brave/browser/ui/tabs/brave_tab_layout_constants.h"
#include "chrome/browser/ui/layout_constants.h"
#include "ui/gfx/geometry/insets.h"

// A subclass of TabStyle used to customize tab layout and visuals. It is
// implemented as a template because it must be included in the source file
// override before the base class definition.
template <typename TabStyleBase>
class BraveTabStyle : public TabStyleBase {
 public:
  int GetTabOverlap() const override {
    return brave_tabs::kHorizontalTabOverlap;
  }

  int GetTopCornerRadius() const override {
    return brave_tabs::kTabBorderRadius;
  }

  int GetBottomCornerRadius() const override {
    return brave_tabs::kTabBorderRadius;
  }

  gfx::Insets GetContentsInsets() const override {
    return gfx::Insets::VH(
        0, brave_tabs::kHorizontalTabPadding + brave_tabs::kHorizontalTabInset);
  }

  int GetPinnedWidth() const override {
    return GetLayoutConstant(TAB_HEIGHT) + brave_tabs::kHorizontalTabInset * 2;
  }
};

#endif  // BRAVE_BROWSER_UI_TABS_BRAVE_TAB_STYLE_H_
