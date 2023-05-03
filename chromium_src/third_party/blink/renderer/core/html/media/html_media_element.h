/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_MEDIA_HTML_MEDIA_ELEMENT_H_
#define BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_MEDIA_HTML_MEDIA_ELEMENT_H_

#include "third_party/blink/renderer/core/dom/events/native_event_listener.h"
#include "third_party/blink/renderer/platform/heap/garbage_collected.h"

#define SetSrc                                                           \
  NotifyEndOfStream();                                                   \
  void GetLoadTypeAndURL(GetLoadTypeAndURLCallback) override;            \
  void CacheCurrentMediaSource(const WTF::String& path,                  \
                               CacheCurrentMediaSourceCallback callback) \
      override;                                                          \
  void SetSrc

#include "src/third_party/blink/renderer/core/html/media/html_media_element.h"

#undef SetSrc

#endif  // BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_HTML_MEDIA_HTML_MEDIA_ELEMENT_H_
