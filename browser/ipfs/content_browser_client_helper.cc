/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ipfs/content_browser_client_helper.h"

#include <string>
#include <utility>

#include "base/strings/string_util.h"
#include "brave/browser/ipfs/ipfs_service_factory.h"
#include "brave/browser/profiles/profile_util.h"
#include "brave/components/constants/url_constants.h"
#include "brave/components/constants/webui_url_constants.h"
#include "brave/components/decentralized_dns/core/utils.h"
#include "brave/components/ipfs/ipfs_constants.h"
#include "brave/components/ipfs/ipfs_utils.h"
#include "brave/components/ipfs/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/external_protocol/external_protocol_handler.h"
#include "chrome/common/channel_info.h"
#include "components/prefs/pref_service.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/common/url_constants.h"
#include "url/gurl.h"
#include "base/debug/stack_trace.h"

namespace {

constexpr char kIpfsLocalhost[] = ".ipfs.localhost";
constexpr char kIpnsLocalhost[] = ".ipns.localhost";

bool IsIPFSLocalGateway(PrefService* prefs) {
  auto resolve_method = static_cast<ipfs::IPFSResolveMethodTypes>(
      prefs->GetInteger(kIPFSResolveMethod));
  return resolve_method == ipfs::IPFSResolveMethodTypes::IPFS_LOCAL;
}

}  // namespace

namespace ipfs {

bool HandleIPFSURLRewrite(GURL* url, content::BrowserContext* browser_context) {
  if (!brave::IsRegularProfile(browser_context)) {
    return false;
  }
//     LOG(INFO) << "[IPFS] HandleIPFSURLRewrite 10"
//   << "\r\n old_url:" << (*url)
//   << "\r\nStack:\r\n" << base::debug::StackTrace()

// ;

    // std::string t = url->spec();
    // if(t.back() == '/'  || t.back() == '\\')
    //   t.pop_back();


//     LOG(INFO) << "[IPFS] HandleIPFSURLRewrite 15 Check for"
//     << "\r\n url:" << url->spec().c_str()
//     << "\r\n t:" << t
//     ;
//     auto* tmp = browser_context->GetUserData(t.c_str());
//     if(tmp != nullptr) {
//       LOG(INFO) << "[IPFS] HandleIPFSURLRewrite 20"
//   << "\r\n new_url:" << (*url)
// ;
//       //return true;
//     }
//   if(t == "ipns://k2k4r8ni09jro03sto91pyi070ww4x63iwub4x3sc13qn5pwkjxhfdt4" || t == "https://drweb.link/ipns/k2k4r8ni09jro03sto91pyi070ww4x63iwub4x3sc13qn5pwkjxhfdt4") {
//     LOG(INFO) << "[IPFS] HandleIPFSURLRewrite 25"
//   << "\r\n old_url:" << (*url)
// ;
// //*url = GURL("https://drweb.link/ipns/k2k4r8ni09jro03sto91pyi070ww4x63iwub4x3sc13qn5pwkjxhfdt4");
//   return false;
// }


  // This is needed for triggering ReverseRewrite later.
  if (url->SchemeIs("http") &&
      (base::EndsWith(url->host_piece(), kIpfsLocalhost) ||
       base::EndsWith(url->host_piece(), kIpnsLocalhost))) {
    return true;
  }
#if BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
  if (url->SchemeIs(content::kChromeUIScheme) && url->DomainIs(kIPFSScheme)) {
    GURL::Replacements host_replacements;
    host_replacements.SetHostStr(kIPFSWebUIHost);
    *url = url->ReplaceComponents(host_replacements);
    return true;
  }
#endif
  PrefService* prefs = user_prefs::UserPrefs::Get(browser_context);
  if (!IsIpfsResolveMethodDisabled(prefs) &&
      // When it's not the local gateway we don't want to show a ipfs:// URL.
      // We instead will translate the URL later.
      IsIPFSLocalGateway(prefs) &&
      (url->SchemeIs(kIPFSScheme) || url->SchemeIs(kIPNSScheme))) {
    
    bool result(TranslateIPFSURI(
        *url, url, GetDefaultIPFSLocalGateway(chrome::GetChannel()), false));
//     LOG(INFO) << "[IPFS] HandleIPFSURLRewrite 30"
//   << "\r\n new_url:" << (*url)
//   << "\r\n result:" << result
// ;
    return result;
  }

  if (url->DomainIs(kLocalhostIP)) {
    GURL::Replacements replacements;
    replacements.SetHostStr(kLocalhostDomain);
    if (IsDefaultGatewayURL(url->ReplaceComponents(replacements), prefs)) {
      *url = url->ReplaceComponents(replacements);
      return true;
    }
  }

  if (IsLocalGatewayConfigured(prefs)) {
    if (decentralized_dns::IsENSTLD(url->host_piece()) &&
        decentralized_dns::IsENSResolveMethodEnabled(
            g_browser_process->local_state())) {
      return true;
    }

    if (decentralized_dns::IsSnsTLD(url->host_piece()) &&
        decentralized_dns::IsSnsResolveMethodEnabled(
            g_browser_process->local_state())) {
      return true;
    }

    if (decentralized_dns::IsUnstoppableDomainsTLD(url->host_piece()) &&
        decentralized_dns::IsUnstoppableDomainsResolveMethodEnabled(
            g_browser_process->local_state())) {
      return true;
    }
  }

//     LOG(INFO) << "[IPFS] HandleIPFSURLRewrite 100"
//   << "\r\n old_url:" << (*url)
// ;

  return false;
}

bool HandleIPFSURLReverseRewrite(GURL* url,
                                 content::BrowserContext* browser_context) {
#if BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
  if (url->SchemeIs(content::kChromeUIScheme) &&
      url->DomainIs(kIPFSWebUIHost)) {
    return true;
  }
#endif

  std::size_t ipfs_pos = url->host_piece().find(kIpfsLocalhost);
  std::size_t ipns_pos = url->host_piece().find(kIpnsLocalhost);

  if (ipfs_pos == std::string::npos && ipns_pos == std::string::npos)
    return false;

  if (auto cid_end = (ipfs_pos == std::string::npos) ? ipns_pos : ipfs_pos;
      !ipfs::IsValidCIDOrDomain(
          ipfs::DecodeSingleLabelForm((url->host().substr(0, cid_end))))) {
    return false;
  }

  GURL configured_gateway = GetConfiguredBaseGateway(
      user_prefs::UserPrefs::Get(browser_context), chrome::GetChannel());
  if (configured_gateway.port() != url->port())
    return false;
  GURL::Replacements scheme_replacements;
  GURL::Replacements host_replacements;
  if (ipfs_pos != std::string::npos) {
    scheme_replacements.SetSchemeStr(kIPFSScheme);
    host_replacements.SetHostStr(url->host_piece().substr(0, ipfs_pos));
    host_replacements.ClearPort();
  } else {  // ipns
    scheme_replacements.SetSchemeStr(kIPNSScheme);
    host_replacements.SetHostStr(url->host_piece().substr(0, ipns_pos));
    host_replacements.ClearPort();
  }

//   LOG(INFO) << "[IPFS] HandleIPFSURLReverseRewrite "
//   << "\r\n old_url:" << (*url)
// ;
  *url = url->ReplaceComponents(host_replacements);
  *url = url->ReplaceComponents(scheme_replacements);
//   LOG(INFO) << "[IPFS] HandleIPFSURLReverseRewrite "
//   << "\r\n new_url:" << (*url)
// ;
  return true;
}

}  // namespace ipfs
