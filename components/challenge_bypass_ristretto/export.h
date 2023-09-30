/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_EXPORT_H_
#define BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_EXPORT_H_

#if defined(COMPONENT_BUILD)
#if defined(WIN32)
#if defined(CHALLENGE_BYPASS_RISTRETTO_IMPLEMENTATION)
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT __declspec(dllexport)
#else
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT __declspec(dllimport)
#endif  // defined(CHALLENGE_BYPASS_RISTRETTO_IMPLEMENTATION)
#else   // defined(WIN32)
#if defined(CHALLENGE_BYPASS_RISTRETTO_IMPLEMENTATION)
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT __attribute__((visibility("default")))
#else
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT
#endif
#endif
#else  // defined(COMPONENT_BUILD)
#define CHALLENGE_BYPASS_RISTRETTO_EXPORT
#endif

#endif  // BRAVE_COMPONENTS_CHALLENGE_BYPASS_RISTRETTO_EXPORT_H_
