// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import {html as polymerHtml, mixinBehaviors, Polymer, PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';

// Global overrides
import 'chrome://brave-resources/br_elements/br_toolbar/br_toolbar.html.js';
import CrButtonStyleTemplate from './overrides/cr_button.js'
import CrToggleStyleTemplate from './overrides/cr_toggle.js'

const debug = false

if (debug) {
  // Useful to diagnose module definition timing for template modification
  // debugging.
  console.log("MODULE: polymer_overriding")
}

const allBehaviorsMap = {}
const allPropertiesMap = {}
const componentPropertyModifications = {}
const ignoredComponents = [
  'cr-toolbar'
]

function addBraveBehaviorsLegacy(moduleName, component) {
  if (allBehaviorsMap[moduleName]) {
    component.behaviors = component.behaviors || []
    component.behaviors.push(...allBehaviorsMap[moduleName])
    delete allBehaviorsMap[moduleName]
  }
}

function addBraveProperties(moduleName, component) {
  if (allPropertiesMap[moduleName]) {
    component.properties = component.properties || {}
    component.properties = {
      ...component.properties,
      ...allPropertiesMap[moduleName]
    }
    delete allPropertiesMap[moduleName]
  } else {
    componentPropertyModifications[moduleName] = component
  }
}

const allBraveTemplateModificationsMap = {}

function addBraveTemplateModifications(moduleName, component, modifyFn) {
  const template = component._template || component.template
  if (template) {
    const templateContent = template.content
    const t0 = debug && performance.now()
    modifyFn(templateContent)
    const t1 = debug && performance.now()
    if (debug)
      console.debug(`Modifying template '${moduleName}' took ${t1 - t0}ms`)
  } else {
    console.error(`Source template not found for override of component "${moduleName}"`)
  }
}

const styleOverridePrefix = 'brave-override-style-'

function addBraveStyleOverride(moduleName, component, template = component._template || component.template) {
  if (!template) {
    console.error(`No template found for component (${moduleName}) with found style overrides`, component)
    return
  }
  const styleElement = template.content.querySelector('style')
  if (!styleElement) {
    console.error(`No style element found for component (${moduleName}) with found style overrides`, component)
    return
  }
  const overrideModuleName = styleOverridePrefix + moduleName
  styleElement.setAttribute(
    'include',
    `${styleElement.getAttribute('include')} ${overrideModuleName}`
  )
  if (debug)
    console.log(`Brave Style Override added for ${moduleName}`, styleElement)
}

export function RegisterPolymerComponentBehaviors(behaviorsMap) {
  if (debug) {
    console.log('RegisterPolymerComponentBehaviors', ...Object.keys(behaviorsMap))
  }
  Object.assign(allBehaviorsMap, behaviorsMap)
}

export function RegisterPolymerComponentProperties(propertiesMap) {
  if (debug) {
    console.debug('RegisterPolymerComponentProperties', ...Object.keys(propertiesMap))
  }
  Object.assign(allPropertiesMap, propertiesMap)
  for (const componentName in propertiesMap) {
    if (componentPropertyModifications[componentName]) {
        addBraveProperties(componentName, componentPropertyModifications[componentName])
    }
  }
}

export function RegisterPolymerTemplateModifications(modificationsMap) {
  if (debug) {
    console.log('RegisterPolymerTemplateModifications', ...Object.keys(modificationsMap))
  }
  const awaitingComponentModifications = {}
  for (const componentName in modificationsMap) {
    const modifyFn = modificationsMap[componentName]
    // const existingComponent = window.customElements.get(componentName)
    // if (!existingComponent) {
    awaitingComponentModifications[componentName] = modificationsMap[componentName]
    // NOTE(petemill): If module has already been defined, it's not ideal as the unmodified
    // template may already have been cloned for an element. However, we assume PolymerElement._prepareTemplate
    // has not been called yet for the component.
    // However, this would be more robust if we moved to a subclassing approach.
  }
  Object.assign(allBraveTemplateModificationsMap, awaitingComponentModifications)
}

export function RegisterPolymerComponentReplacement(name, component) {
  if (debug) {
    console.log(`RegisterPolymerComponentReplacement: ${name}`)
  }
  if (!ignoredComponents.includes(name)) {
    console.warn(`RegisterPolymerComponentReplacement: did not find component '${name}' as being ignored via RegisterPolymerComponentToIgnore`)
  }
  define(name, component)
}

export function RegisterPolymerComponentToIgnore(name) {
  if (debug) {
    console.log(`RegisterPolymerComponentToIgnore ${name}`)
  }
  ignoredComponents.push(name)
}

const moduleNamesWithStyleOverrides = []

export async function RegisterStyleOverride(componentName, styleTemplate) {
  // Inform polymer of styles by creating a dom-module.
  //   <dom-module id="${styleOverridePrefix + componentName}">
  //     <template>
  //       ${styleOuterHTML}
  //     </template>
  //   </dom-module>
  // This is the same way that polymer_modulizer uses for style-module types.
  const template = document.createElement('template')
  const domModule = document.createElement('dom-module')
  domModule.id = styleOverridePrefix + componentName
  template.appendChild(domModule)
  domModule.appendChild(styleTemplate)
  document.body.appendChild(template.content.cloneNode(true))
  if (debug) {
    console.log(`REGISTERING STYLE OVERRIDE for ${componentName}`, styleTemplate)
  }
  // NOTE(petemill): If module has already been defined, it's not ideal as the unmodified
  // template may already have been cloned for an element. However, we assume PolymerElement._prepareTemplate
  // has not been called yet for the component.
  // However, this would be more robust if we moved to a subclassing approach.
  moduleNamesWithStyleOverrides.push(componentName)
}

export function OverrideIronIcons(iconSetName, overridingIconSetName, iconOverrides) {
  // Here, we change the underlying DOM.
  // We cannot simply change `IconSet._icons` since
  // there are occasions where this will get re-parsed from DOM elements.
  // const meta = Base.create('iron-meta', {type: 'iconset'})
  const meta = document.createElement('iron-meta')
  meta.type = 'iconset'
  const srcIconSet = meta.byKey(iconSetName)
  if (!srcIconSet) {
    console.error(`OverrideIronIcons: source icon set "${iconSetName} not found.`)
    return
  }
  const overrideIconSet = meta.byKey(overridingIconSetName)
  if (!overrideIconSet) {
    console.error(`OverrideIronIcons: overriding icon set "${overridingIconSetName} not found.`)
    return
  }
  for (const chromiumIconName in iconOverrides) {
    const chromiumIcon = srcIconSet.querySelector(`#${chromiumIconName}`)
    if (!chromiumIcon) {
      console.error(`[brave overrides] Could not find chromium icon '${chromiumIconName}' in iconset '${iconSetName}' for replacement!`)
      continue
    }
    const braveIconName = iconOverrides[chromiumIconName]
    const braveIcon = overrideIconSet.querySelector(`#${braveIconName}`)
    if (!braveIcon) {
      console.error(`[brave overrides] Could not find brave icon '${braveIconName}' in iconset '${overridingIconSetName}' for replacement!`)
      continue
    }
    // replace
    while (chromiumIcon.firstChild) {
      chromiumIcon.firstChild.remove()
    }
    while (braveIcon.firstChild) {
      chromiumIcon.appendChild(braveIcon.firstChild)
    }
  }
  // Ensure icons get re-parsed if already parseds
  // `getIconNames` ensures this._icons in iron-iconset-svg is re-parsed
  // from DOM. If that changes, we'll need to find another way,
  // perhaps `srcIconSet._icons = srcIconSet._createIconMap()`
  srcIconSet.getIconNames()
}

// Overriding Polymer.Class only works for some
// chromium components which call Polymer() and pass objects rather
// than classes.
const oldClass = Polymer.Class
Polymer.Class = function (info, mixin) {
  if (!info) {
    console.warn('Polymer.Class requires `info` argument');
  }
  const name = info.is
  if (!name) {
    if (debug) {
      console.warn(`Non-polymer-like polymer class`, info)
    }
    return oldClass(info, mixin)
  }
  if (debug) {
    console.debug(`Polymer component legacy registering: ${name}`, info)
  }
  addBraveBehaviorsLegacy(name, info)
  return oldClass(info, mixin)
}

// Also override for components which do not call Polymer() but instead
// inherit from PolymerElement.
const oldPrepareTemplate = PolymerElement._prepareTemplate;
PolymerElement._prepareTemplate = function BravePolymer_PrepareTemplate() {
  oldPrepareTemplate.call(this)
  const name = this.is
  if (!name) {
    if (debug) {
      console.warn('PolymerElement defined with no name', this, this.prototype)
    }
    return
  }
  if (debug) {
    console.log('PolymerElement _prepareTemplate: ', name, this, this.prototype)
  }
  // Perform modifications that we want to change the original class / prototype
  // features, such as editing template or properties.
  // Other modifications, such as injecting overriden classes (aka behaviors),
  // will happen at component definition time.
  addBraveProperties(name, this.prototype)
  const templateModifyFn = allBraveTemplateModificationsMap[name]
  if (templateModifyFn) {
    addBraveTemplateModifications(name, this.prototype, templateModifyFn)
    // TODO(petemill): delete allBraveTemplateModificationsMap entry when done so that the
    // function can be collected. We do not delete at the moment since
    // _prepareTemplate can be called multiple times. We should move template
    // modification to happen via automatic subclassing and overriding of template
    // property.
  }
  if (moduleNamesWithStyleOverrides.includes(name)) {
    addBraveStyleOverride(name, this.prototype)
  }
}

const oldDefine = window.customElements.define

function BraveDefineCustomElements (name, component, options, useIgnoreList = true) {
  if (component.polymerElementVersion) {
    if (debug) {
      console.log('BraveDefineCustomElements PolymerElement defined', name, component, options)
    }
    // Global ignore (likely due to manual replacement)
    if (useIgnoreList && ignoredComponents.includes(name)) {
      if (debug) {
        console.log(`BraveDefineCustomElements ignored ${name}`)
      }
      return
    }
    // Inject behaviors
    if (allBehaviorsMap[name]) {
      if (debug) {
        console.log('BraveDefineCustomElements added behavior', allBehaviorsMap[name])
      }
      component = mixinBehaviors(allBehaviorsMap[name], component)
      delete allBehaviorsMap[name]
    }
  }
  oldDefine.call(this, name, component, options)
}

window.customElements.define = BraveDefineCustomElements

export function define (name, component, options) {
  // We still want style and template overrides
  BraveDefineCustomElements.call(window.customElements, name, component, options, false)
}

/**
 * Allows regular string tagged templating before parsing as html elements.
 * Differs from polymer's html function only in that it allows strings and
 * does not allow nested htmlelements.
 * Named html for syntax highlighting in some IDEs.
 * @param {*} strings
 * @param  {...any} values
 * @returns HTMLTEmplateElement
 */
 export function html(strings, ...values) {
  // Get regular string placeholders first (basic `i am ${'a'} string` parsing)
  // since Polymer's html tagged template only supports html element
  // placeholders.
  const htmlRaw = values.reduce((acc, v, idx) =>
      acc + v + strings[idx + 1], strings[0])
  // Utilize polymer's tagged template element creation for no other reason than we are allowed
  // to call innerHTML there.
  return polymerHtml([htmlRaw]).content.cloneNode(true)
}

// Overrides for all pages
RegisterStyleOverride('cr-toggle', CrToggleStyleTemplate)
RegisterStyleOverride('cr-button', CrButtonStyleTemplate)
