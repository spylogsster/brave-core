/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/platform/media/web_source_buffer_impl.h"

#include <vector>

#include "base/files/file_util.h"
#include "media/filters/chunk_demuxer.h"

namespace {

// TODO(sko) Move to member.
std::vector<const unsigned char> g_cache;

bool CopyToCache(const unsigned char* data, size_t length) {
  g_cache = std::vector<const unsigned char>(data, data + length);
  return true;
}

}  // namespace

// Leave this version as is. This is expected to be function definition.
#define AppendToParseBuffer_With2Args(...) AppendToParseBuffer(__VA_ARGS__)

// This version is invocation of demuxer and we want to override this.
#define AppendToParseBuffer_With3Args(ID, DATA, LENGTH) \
  AppendToParseBuffer(ID, DATA, LENGTH) && (CopyToCache(DATA, LENGTH))

// When arg num is two, this would be expanded like
//
//  "foo", "bar", AppendToParseBuffer_With3Args, AppendToParseBuffer_With2Args
//                                                ^ MACRO_TO_USE
//
// When arg num is three, this would be expanded like
//  "foo", "bar", "baz", AppendToParseBuffer_With3Args, ...
//                         ^ MACRO_TO_USE
//
#define AppendToParseBuffer_Disambiguation(ARG_1, ARG_2, ARG_3, MACRO_TO_USE, \
                                           ...)                               \
  MACRO_TO_USE
#define AppendToParseBuffer(...)                  \
  AppendToParseBuffer_Disambiguation(             \
      __VA_ARGS__, AppendToParseBuffer_With3Args, \
      AppendToParseBuffer_With2Args)(__VA_ARGS__)

#include "src/third_party/blink/renderer/platform/media/web_source_buffer_impl.cc"

namespace blink {

void WebSourceBufferImpl::WriteToFile(const WTF::String& path,
                                      base::OnceCallback<void(bool)> callback) {
  auto result = base::WriteFile(
      base::FilePath(FILE_PATH_LITERAL("./test45.mp4")),
      reinterpret_cast<const char*>(g_cache.data()), g_cache.size());
  std::move(callback).Run(result != -1);
}

}  // namespace blink
