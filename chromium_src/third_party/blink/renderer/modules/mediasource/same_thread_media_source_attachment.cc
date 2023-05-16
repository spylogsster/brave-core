/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/third_party/blink/renderer/modules/mediasource/same_thread_media_source_attachment.cc"

namespace blink {

void SameThreadMediaSourceAttachment::NotifyEndOfStream(
    MediaSourceTracer* tracer) {
  GetMediaElement(tracer)->NotifyEndOfStream();
}

Blob* SameThreadMediaSourceAttachment::GetBufferCache(
    MediaSourceTracer* tracer) {
  auto* media_source = GetMediaSource(tracer);
  if (!media_source) {
    DVLOG(2) << "Are we in the middle of detaching process?";
    return nullptr;
  }

  auto* source_buffers = media_source->activeSourceBuffers();
  if (!source_buffers || source_buffers->length() == 0) {
    DVLOG(2) << "source buffer might be removed.";
    return nullptr;
  }

  if (source_buffers->length() > 1) {
    DVLOG(2) << "There're multiple source buffers. we don't have ability to "
                "mixing them into one file.";
    // TODO(sko) What should we do for this case? Should we cache multiple
    // files? or mixing them into one file? The latter would be quite tricky.
    return nullptr;
  }

  auto* source_buffer = source_buffers->item(0);
  DCHECK(source_buffer);

  const auto& cached_buffer = source_buffer->GetBufferCache();
  return Blob::Create(
      reinterpret_cast<const unsigned char*>(cached_buffer->data()),
      cached_buffer->length(), "video/mp4");
}

}  // namespace blink
