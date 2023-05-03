/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CONTENT_BROWSER_MEDIA_MEDIA_WEB_CONTENTS_OBSERVER_H_
#define BRAVE_CHROMIUM_SRC_CONTENT_BROWSER_MEDIA_MEDIA_WEB_CONTENTS_OBSERVER_H_

#include "content/browser/media/session/media_session_controllers_manager.h"
#include "media/mojo/mojom/media_player.mojom.h"

#define OnAudioOutputSinkChangingDisabled       \
  OnAudioOutputSinkChangingDisabled() override; \
  void OnEndOfStream

#include "src/content/browser/media/media_web_contents_observer.h"

#undef OnAudioOutputSinkChangingDisabled

#endif  // BRAVE_CHROMIUM_SRC_CONTENT_BROWSER_MEDIA_MEDIA_WEB_CONTENTS_OBSERVER_H_
