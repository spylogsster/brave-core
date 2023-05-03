/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASOURCE_SAME_THREAD_MEDIA_SOURCE_ATTACHMENT_H_
#define BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASOURCE_SAME_THREAD_MEDIA_SOURCE_ATTACHMENT_H_

#include "third_party/blink/renderer/core/html/media/media_source_attachment.h"

#define VerifyCalledWhileContextsAliveForDebugging     \
  VerifyCalledWhileContextsAliveForDebugging_Unused(); \
  void NotifyEndOfStream(MediaSourceTracer*) final;    \
  void WriteCurrentSourceBufferToFile(                 \
      MediaSourceTracer*, const WTF::String& path,     \
      base::OnceCallback<void(bool)> callback) final;  \
  void VerifyCalledWhileContextsAliveForDebugging

#include "src/third_party/blink/renderer/modules/mediasource/same_thread_media_source_attachment.h"

#undef VerifyCalledWhileContextsAliveForDebugging

#endif  // BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_MEDIASOURCE_SAME_THREAD_MEDIA_SOURCE_ATTACHMENT_H_
