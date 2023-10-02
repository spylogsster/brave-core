// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_PSST_CORE_PSST_COMPONENT_INSTALLER_H_
#define BRAVE_COMPONENTS_PSST_CORE_PSST_COMPONENT_INSTALLER_H_

namespace component_updater {
class ComponentUpdateService;
}  // namespace component_updater

namespace psst {

void RegisterPsstComponent(component_updater::ComponentUpdateService* cus);

}  // namespace psst

#endif  // BRAVE_COMPONENTS_PSST_CORE_PSST_COMPONENT_INSTALLER_H_
