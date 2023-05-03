/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/third_party/blink/renderer/modules/mediasource/cross_thread_media_source_attachment.cc"

namespace blink {

void CrossThreadMediaSourceAttachment::NotifyEndOfStream(
    MediaSourceTracer* tracer) {
  // TODO(sko) Post notification to main thread using main_runner_.
  // NOTIMPLEMENTED();
}

void CrossThreadMediaSourceAttachment::WriteCurrentSourceBufferToFile(
    MediaSourceTracer* tracer,
    const WTF::String& path,
    base::OnceCallback<void(bool)> callback) {
  // TODO(sko) Post task to write file.
  // NOTIMPLEMENTED();
}

}  // namespace blink
