/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_MANAGER_H_
#define BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_MANAGER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "base/synchronization/lock.h"
#include "base/thread_annotations.h"
#include "base/values.h"
#include "brave/components/brave_component_updater/browser/brave_component.h"
#include "brave/components/brave_shields/browser/ad_block_component_filters_provider.h"
#include "brave/components/brave_shields/browser/ad_block_engine.h"
#include "brave/components/brave_shields/browser/ad_block_filter_list_catalog_provider.h"
#include "brave/components/brave_shields/browser/ad_block_filters_provider_manager.h"
#include "brave/components/brave_shields/browser/ad_block_resource_provider.h"
#include "brave/components/brave_shields/browser/ad_block_service.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"

class AdBlockServiceTest;

using brave_component_updater::BraveComponent;

namespace brave_shields {

class AdBlockRegionalService;
class FilterListCatalogEntry;

// The AdBlock regional service manager, in charge of initializing and
// managing regional AdBlock clients.
class AdBlockRegionalServiceManager
    : public AdBlockFilterListCatalogProvider::Observer {
 public:
  explicit AdBlockRegionalServiceManager(
      AdBlockFiltersProviderManager* filters_manager,
      PrefService* local_state,
      std::string locale,
      component_updater::ComponentUpdateService* cus,
      scoped_refptr<base::SequencedTaskRunner> task_runner);
  AdBlockRegionalServiceManager(const AdBlockRegionalServiceManager&) = delete;
  AdBlockRegionalServiceManager& operator=(
      const AdBlockRegionalServiceManager&) = delete;
  ~AdBlockRegionalServiceManager() override;

  base::Value::List GetRegionalLists();

  void SetFilterListCatalog(std::vector<FilterListCatalogEntry> catalog);
  const std::vector<FilterListCatalogEntry>& GetFilterListCatalog();

  bool Start();
  bool IsFilterListAvailable(const std::string& uuid) const;
  bool IsFilterListEnabled(const std::string& uuid) const;
  void EnableFilterList(const std::string& uuid, bool enabled);

  void Init(AdBlockResourceProvider* resource_provider,
            AdBlockFilterListCatalogProvider* catalog_provider);
  bool IsInitialized();

  // AdBlockFilterListCatalogProvider::Observer
  void OnFilterListCatalogLoaded(const std::string& catalog_json) override;

 private:
  friend class ::AdBlockServiceTest;
  void StartRegionalServices();
  void UpdateFilterListPrefs(const std::string& uuid, bool enabled);

  void RecordP3ACookieListEnabled();

  raw_ptr<PrefService> local_state_;
  std::string locale_;
  bool initialized_;
  base::Lock regional_services_lock_;
  std::map<std::string, std::unique_ptr<AdBlockComponentFiltersProvider>>
      regional_filters_providers_;

  std::vector<FilterListCatalogEntry> filter_list_catalog_;

  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  raw_ptr<component_updater::ComponentUpdateService> component_update_service_;
  raw_ptr<AdBlockFiltersProviderManager> filters_manager_;
  raw_ptr<AdBlockResourceProvider> resource_provider_;
  raw_ptr<AdBlockFilterListCatalogProvider> catalog_provider_;

  base::WeakPtrFactory<AdBlockRegionalServiceManager> weak_factory_{this};
};

// Creates the AdBlockRegionalServiceManager
std::unique_ptr<AdBlockRegionalServiceManager>
AdBlockRegionalServiceManagerFactory(
    AdBlockFiltersProviderManager* filters_manager,
    PrefService* local_state,
    std::string locale,
    component_updater::ComponentUpdateService* cus,
    scoped_refptr<base::SequencedTaskRunner> task_runner);

}  // namespace brave_shields

#endif  // BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_AD_BLOCK_REGIONAL_SERVICE_MANAGER_H_
