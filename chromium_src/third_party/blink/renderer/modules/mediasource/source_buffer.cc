/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/third_party/blink/renderer/modules/mediasource/source_buffer.cc"

namespace blink {

void SourceBuffer::WriteToFile(const WTF::String& path,
                               base::OnceCallback<void(bool)> callback) {
  if (!web_source_buffer_) {
    LOG(ERROR) << "Can this happen?";
    std::move(callback).Run(false);
  }

  web_source_buffer_->WriteToFile(path, std::move(callback));
}

}  // namespace blink
