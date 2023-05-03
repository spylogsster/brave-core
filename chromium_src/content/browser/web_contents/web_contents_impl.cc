/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/content/browser/web_contents/web_contents_impl.cc"

#include "base/logging.h"

#undef DVLOG
#define DVLOG(i) LOG(ERROR)

namespace content {

void WebContentsImpl::MediaPlayerCreated(const MediaPlayerId& id) {
  DVLOG(2) << __FUNCTION__;
  // GetMediaLoadTypeAndURL(
  //     id,
  //     base::BindOnce([](media::mojom::LoadType type, const std::string& url)
  //     {
  //       DVLOG(2) << "GetLoadTypeAndRemote response: " << type << " " << url;
  //     }));

  for (auto& observer : observers_.observer_list()) {
    observer.MediaPlayerCreated(this, id);
  }
}

void WebContentsImpl::GetMediaLoadTypeAndURL(
    const MediaPlayerId& id,
    media::mojom::MediaPlayer::GetLoadTypeAndURLCallback callback) {
  DVLOG(2) << __FUNCTION__;

  DCHECK(media_web_contents_observer_->IsMediaPlayerRemoteAvailable(id));

  auto& player_remote = media_web_contents_observer_->GetMediaPlayerRemote(id);
  DCHECK(player_remote.is_bound());

  player_remote->GetLoadTypeAndURL(std::move(callback));
}

void WebContentsImpl::MediaPlayerEndOfStream(const MediaPlayerId& id) {
  DVLOG(2) << __FUNCTION__;

  for (auto& observer : observers_.observer_list()) {
    observer.MediaPlayerEndOfStream(this, id);
  }
}

}  // namespace content
