/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_MEDIA_MEDIA_SOURCE_ATTACHMENT_H_
#define BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_MEDIA_MEDIA_SOURCE_ATTACHMENT_H_

namespace blink {
class MediaSource;
}  // namespace blink

#define OnTrackChanged                                    \
  OnTrackChanged_Unused() {}                              \
  virtual void NotifyEndOfStream(MediaSourceTracer*) = 0; \
  virtual void WriteCurrentSourceBufferToFile(            \
      MediaSourceTracer*, const WTF::String& path,        \
      base::OnceCallback<void(bool)> callback) = 0;       \
  virtual void OnTrackChanged

#include "src/third_party/blink/renderer/core/html/media/media_source_attachment.h"

#undef OnTrackChanged

#endif  // BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_MEDIA_MEDIA_SOURCE_ATTACHMENT_H_
