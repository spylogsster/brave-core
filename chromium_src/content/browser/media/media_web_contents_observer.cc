/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BRAVE_MEDIA_WEB_CONTENTS_OBSERVER_ON_MEDIA_PLAYER_ADDED \
  web_contents_impl()->MediaPlayerCreated(player_id);

#include "src/content/browser/media/media_web_contents_observer.cc"

#undef BRAVE_MEDIA_WEB_CONTENTS_OBSERVER_ON_MEDIA_PLAYER_ADDED

namespace content {

void MediaWebContentsObserver::MediaPlayerObserverHostImpl::OnEndOfStream() {
  media_web_contents_observer_->web_contents_impl()->MediaPlayerEndOfStream(
      media_player_id_);
}

}  // namespace content
