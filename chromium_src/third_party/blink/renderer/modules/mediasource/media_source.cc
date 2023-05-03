/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "third_party/blink/public/platform/web_media_source.h"

#define MarkEndOfStream(EOS_STATUS)                              \
  MarkEndOfStream(EOS_STATUS);                                   \
  if (EOS_STATUS == WebMediaSource::kEndOfStreamStatusNoError) { \
    auto [attachment, tracer] = AttachmentAndTracer();           \
    attachment->NotifyEndOfStream(tracer);                       \
  }

#include "src/third_party/blink/renderer/modules/mediasource/media_source.cc"
