// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_COMPONENT_INSTALLER_H_
#define BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_COMPONENT_INSTALLER_H_

#include <string>

#include "base/files/file_path.h"
#include "base/functional/callback.h"

const char kAdBlockDefaultComponentName[] = "Brave Ad Block Updater";
const char kAdBlockDefaultComponentId[] = "iodkpdagapdfkphljnddpjlldadblomo";
const char kAdBlockDefaultComponentBase64PublicKey[] =
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsD/B/MGdz0gh7WkcFARn"
    "ZTBX9KAw2fuGeogijoI+fET38IK0L+P/trCT2NshqhRNmrDpLzV2+Dmes6PvkA+O"
    "dQkUV6VbChJG+baTfr3Oo5PdE0WxmP9Xh8XD7p85DQrk0jJilKuElxpK7Yq0JhcT"
    "Sc3XNHeTwBVqCnHwWZZ+XysYQfjuDQ0MgQpS/s7U04OZ63NIPe/iCQm32stvS/pE"
    "ya7KdBZXgRBQ59U6M1n1Ikkp3vfECShbBld6VrrmNrl59yKWlEPepJ9oqUc2Wf2M"
    "q+SDNXROG554RnU4BnDJaNETTkDTZ0Pn+rmLmp1qY5Si0yGsfHkrv3FS3vdxVozO"
    "PQIDAQAB";

namespace component_updater {
class ComponentUpdateService;
}  // namespace component_updater

namespace brave_shields {

using OnComponentReadyCallback =
    base::RepeatingCallback<void(const base::FilePath& install_path)>;

void RegisterAdBlockDefaultComponent(
    component_updater::ComponentUpdateService* cus,
    OnComponentReadyCallback callback);

void RegisterAdBlockDefaultResourceComponent(
    component_updater::ComponentUpdateService* cus,
    OnComponentReadyCallback callback);

void RegisterAdBlockFilterListCatalogComponent(
    component_updater::ComponentUpdateService* cus,
    OnComponentReadyCallback callback);

void RegisterAdBlockFiltersComponent(
    component_updater::ComponentUpdateService* cus,
    const std::string& component_public_key,
    const std::string& component_id,
    const std::string& component_name,
    OnComponentReadyCallback callback);

}  // namespace brave_shields

#endif  // BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_COMPONENT_INSTALLER_H_
