/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASOURCE_SOURCE_BUFFER_H_
#define BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASOURCE_SOURCE_BUFFER_H_

#define setTrackDefaults                                     \
  setTrackDefaults_Unused();                                 \
  void WriteToFile(const WTF::String& path,                  \
                   base::OnceCallback<void(bool)> callback); \
  void setTrackDefaults

#include "src/third_party/blink/renderer/modules/mediasource/source_buffer.h"

#undef setTrackDefaults

#endif  // BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASOURCE_SOURCE_BUFFER_H_
