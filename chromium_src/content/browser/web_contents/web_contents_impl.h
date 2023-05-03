/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CHROMIUM_SRC_CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_IMPL_H_
#define CHROMIUM_SRC_CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_IMPL_H_

#define MediaPlayerSeek                                               \
  GetMediaLoadTypeAndURL(                                             \
      const MediaPlayerId& id,                                        \
      media::mojom::MediaPlayer::GetLoadTypeAndURLCallback callback); \
  void CacheCurrentMediaSource() {}                                   \
  void MediaPlayerSeek(const MediaPlayerId& id);                      \
  void MediaPlayerCreated(const MediaPlayerId& id);                   \
  void MediaPlayerEndOfStream

#include "src/content/browser/web_contents/web_contents_impl.h"

#undef MediaPlayerSeek

#endif  // CHROMIUM_SRC_CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_IMPL_H_
