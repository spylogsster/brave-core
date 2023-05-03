/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_SOURCE_BUFFER_H_
#define BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_SOURCE_BUFFER_H_

#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

#define SetClient                                               \
  SetClient_Unused() {}                                         \
  virtual void WriteToFile(const WTF::String& path,             \
                           base::OnceCallback<void(bool)>) = 0; \
  virtual void SetClient

#include "src/third_party/blink/public/platform/web_source_buffer.h"

#undef SetClient

#endif  // BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_SOURCE_BUFFER_H_
