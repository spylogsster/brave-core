/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_OBSERVER_H_
#define BRAVE_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_OBSERVER_H_

#define AboutToBeDiscarded                                               \
  AboutToBeDiscarded_Unused() {}                                         \
  virtual void MediaPlayerCreated(WebContents* web_contents,             \
                                  const MediaPlayerId& player_id) {}     \
  virtual void MediaPlayerEndOfStream(WebContents* web_contents,         \
                                      const MediaPlayerId& player_id) {} \
  virtual void AboutToBeDiscarded

#include "src/content/public/browser/web_contents_observer.h"

#undef AboutToBeDiscarded

#endif  // BRAVE_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_OBSERVER_H_
