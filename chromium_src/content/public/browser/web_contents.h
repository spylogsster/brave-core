/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_H_
#define BRAVE_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_H_

#include "content/public/browser/media_player_id.h"

#define ClosePage                                                 \
  GetCachedMediaSourceURL(                                        \
      const content::MediaPlayerId& id,                           \
      base::OnceCallback<void(const std::string&)> callback) = 0; \
  virtual void ClosePage

#include "src/content/public/browser/web_contents.h"

#undef ClosePage

#endif  // BRAVE_CHROMIUM_SRC_CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_H_
