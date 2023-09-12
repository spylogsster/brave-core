/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/net/brave_torrent_redirect_network_delegate_helper.h"

#include <memory>
#include <string>

#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "brave/components/brave_webtorrent/browser/webtorrent_util.h"
#include "brave/components/constants/network_constants.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/site_instance.h"
#include "content/public/browser/web_contents.h"
#include "extensions/common/constants.h"
#include "net/http/http_content_disposition.h"
#include "net/http/http_response_headers.h"
#include "net/url_request/url_request.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "ui/base/page_transition_types.h"
#include "url/origin.h"

namespace {

// Returns true if the URL contains a URL fragment that starts with "ix=". For
// example, https://webtorrent.io/torrents/big-buck-bunny.torrent#ix=1.
bool IsViewerURL(const GURL& url) {
  return base::StartsWith(url.ref(),
                          "ix=", base::CompareCase::INSENSITIVE_ASCII);
}

bool IsWebtorrentInitiated(std::shared_ptr<brave::BraveRequestInfo> ctx) {
  return ctx->initiator_url.scheme() == extensions::kExtensionScheme &&
         ctx->initiator_url.host() == brave_webtorrent_extension_id;
}

// Returns true if the resource type is a frame (i.e. a top level page) or a
// subframe (i.e. a frame or iframe). For all other resource types (stylesheet,
// script, XHR request, etc.), returns false.
bool IsMainFrameResource(std::shared_ptr<brave::BraveRequestInfo> ctx) {
  return ctx->resource_type == blink::mojom::ResourceType::kMainFrame;
}

}  // namespace

namespace webtorrent {

int OnHeadersReceived_TorrentRedirectWork(
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    GURL* allowed_unsafe_redirect_url,
    const brave::ResponseCallback& next_callback,
    std::shared_ptr<brave::BraveRequestInfo> ctx) {
  if (!original_response_headers || !IsMainFrameResource(ctx) ||
      ctx->is_webtorrent_disabled ||
      // download .torrent, do not redirect
      (IsWebtorrentInitiated(ctx) && !IsViewerURL(ctx->request_url)) ||
      !IsTorrentFile(ctx->request_url, original_response_headers)) {
    return net::OK;
  }

  auto* contents =
      content::WebContents::FromFrameTreeNodeId(ctx->frame_tree_node_id);
  GURL url(base::StrCat(
      {extensions::kExtensionScheme, "://", brave_webtorrent_extension_id,
       "/extension/brave_webtorrent2.html?", ctx->request_url.spec()}));
  content::NavigationController::LoadURLParams params(url);
  params.is_renderer_initiated = true;
  params.initiator_origin = url::Origin::Create(url);
  params.source_site_instance = content::SiteInstance::CreateForURL(
      contents->GetBrowserContext(), params.initiator_origin->GetURL());
  params.transition_type = ui::PAGE_TRANSITION_FROM_API;

  contents->GetController().LoadURLWithParams(params);

  return net::OK;
}

}  // namespace webtorrent
