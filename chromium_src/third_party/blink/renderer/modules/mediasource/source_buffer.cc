/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/third_party/blink/renderer/modules/mediasource/source_buffer.cc"

namespace blink {

const scoped_refptr<RawData>& SourceBuffer::GetBufferCache() const {
  if (!web_source_buffer_) {
    NOTREACHED() << "Can this happen?";
  }

  return web_source_buffer_->GetBufferCache();
}

}  // namespace blink
