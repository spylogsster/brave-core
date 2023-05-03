/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/core/html/media/html_media_element.h"

#include "third_party/blink/renderer/core/html/media/autoplay_policy.h"

#include "base/logging.h"
#undef DVLOG
#define DVLOG(i) LOG(ERROR)

// Do not pause video if it was blocked by autoplay policy on volume change.
bool SkipPauseIfAutoplayIsBlockedByPolicy(bool is_gesture_needed) {
  if (is_gesture_needed)
    return false;
  return true;
}

#define RequestAutoplayUnmute                                \
  RequestAutoplayUnmute() &&                                 \
      SkipPauseIfAutoplayIsBlockedByPolicy(                  \
          autoplay_policy_->IsGestureNeededForPlayback()) && \
      EffectiveMediaVolume

#define BRAVE_HTML_MEDIA_ELEMENT_TRACE \
  visitor->Trace(media_source_event_listener_);

#include "src/third_party/blink/renderer/core/html/media/html_media_element.cc"

#undef RequestAutoplayUnmute

namespace blink {

void HTMLMediaElement::GetLoadTypeAndURL(GetLoadTypeAndURLCallback callback) {
  DVLOG(2) << __FUNCTION__;

  // Unfortunately, mojo seems to forbid to pass WTF::String(). We should pass
  // "" instead.
  WTF::String url = "";
  switch (GetLoadType()) {
    case WebMediaPlayer::kLoadTypeURL: {
      if (!current_src_.GetSourceIfVisible().IsNull()) {
        url = downloadURL();
      }
      std::move(callback).Run(media::mojom::LoadType::URL, url);
      return;
    }
    case WebMediaPlayer::kLoadTypeMediaSource: {
      std::move(callback).Run(media::mojom::LoadType::MediaSource, url);
      return;
    }
    case WebMediaPlayer::kLoadTypeMediaStream: {
      std::move(callback).Run(media::mojom::LoadType::MediaStream, url);
      return;
    }
    default:
      NOTREACHED();
  }
}

void HTMLMediaElement::CacheCurrentMediaSource(
    const WTF::String& path,
    CacheCurrentMediaSourceCallback callback) {
  if (!media_source_attachment_) {
    DVLOG(2) << "Media source seems to be detached.";
    std::move(callback).Run(false);
  }

  media_source_attachment_->WriteCurrentSourceBufferToFile(
      media_source_tracer_, path, std::move(callback));
}

void HTMLMediaElement::NotifyEndOfStream() {
  for (auto& observer : media_player_observer_remote_set_->Value()) {
    observer->OnEndOfStream();
  }

  // REMOVE:
  CacheCurrentMediaSource("", base::BindOnce([](bool result) {
                            LOG(ERROR)
                                << "DUMP SOURCE BUFFER RESULT: " << result;
                          }));
}

}  // namespace blink
