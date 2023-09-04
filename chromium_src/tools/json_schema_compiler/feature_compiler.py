# Copyright (c) 2023 The Brave Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.

#pylint: disable=line-too-long,too-few-public-methods,undefined-variable,protected-access

import override_utils


# echo -n mnojpmjdmbbfmejpflffifhffcmidifd | openssl sha1 | tr '[:lower:]' '[:upper:]'
class BraveExtensions:
    BRAVE_SHIELDS = "A321D47A2B4CA86898167A55CA8B2E02385EA7CD"  # mnojpmjdmbbfmejpflffifhffcmidifd
    ETHEREUM_REMOTE_CLIENT = "21070F3D60711361C1210B870439BE49B5D995F4"
    IPFS_COMPANION = "780BF954C0F7C586EA9662D4F967771F49CC2114"  # nibjojkomfdiaoajekhjakgkdhaomnch
    IPFS_COMPANION_BETA = "FF32507DC3DB5DFFD1D6733187C84D4B74713D63"  # hjoieblefckbooibpepigmacodalfndh
    WEB_TORRENT = "3D9518A72EB02667A773B69DBA9E72E0F4A37423"  # lgjmpdmojkpocjcopdikifhejkkjglho


def EnsureFileIsBraveKnown(source_file):
    # This is executed for all files. List all of them so we can fail in case
    # something new appears in the repository or if an existing file is being
    # moved somewhere.
    known_files = {
        "chrome/common/apps/platform_apps/api/_api_features.json",
        "chrome/common/apps/platform_apps/api/_permission_features.json",
        "chrome/common/controlled_frame/api/_api_features.json",
        "chrome/common/extensions/api/_api_features.json",
        "chrome/common/extensions/api/_manifest_features.json",
        "chrome/common/extensions/api/_permission_features.json",
        "extensions/common/api/_api_features.json",
        "extensions/common/api/_behavior_features.json",
        "extensions/common/api/_manifest_features.json",
        "extensions/common/api/_permission_features.json",
    }

    if source_file not in known_files:
        raise RuntimeError(
            f"Unknown features file {source_file}. Please update "
            "brave/chromium_src/tools/json_schema_compiler/feature_compiler.py")


def GetBraveFeatureExtensions(source_file):
    feature_extensions = {
        "chrome/common/extensions/api/_api_features.json": {
            "extension.inIncognitoContext": [
                {
                    "channel": "stable",
                    "contexts": ["webui"],
                    "matches": [
                        "chrome://newtab/*",
                    ]
                },
            ],
            "bookmarks": [
                {
                    "channel": "stable",
                    "contexts": ["webui"],
                    "matches": [
                        "chrome://newtab/*",
                    ]
                },
            ],
            "tabs": [
                {
                    "channel": "stable",
                    "contexts": ["webui"],
                    "matches": [
                        "chrome://brave-shields.top-chrome/*",
                        "chrome://rewards-panel.top-chrome/*",
                        "chrome://wallet-panel.top-chrome/*",
                        "chrome://wallet/*",
                    ]
                },
            ],
            "topSites": [
                {
                    "channel": "stable",
                    "contexts": ["webui"],
                    "matches": [
                        "chrome://newtab/*",
                    ]
                },
            ],
        },
        "chrome/common/extensions/api/_permission_features.json": {
            "settingsPrivate": [
                {
                    "channel": "stable",
                    "extension_types": ["extension"],
                    "allowlist": [
                        BraveExtensions.BRAVE_SHIELDS,
                    ]
                },
            ],
        },
        "extensions/common/api/_api_features.json": {
            "sockets.tcp": [
                {
                    "dependencies": ["manifest:sockets"],
                    "contexts": ["blessed_extension"],
                    "allowlist": [
                        BraveExtensions.IPFS_COMPANION,
                        BraveExtensions.IPFS_COMPANION_BETA,
                        BraveExtensions.WEB_TORRENT,
                    ]
                },
            ],
            "sockets.tcpServer": [
                {
                    "dependencies": ["manifest:sockets"],
                    "contexts": ["blessed_extension"],
                    "allowlist": [
                        BraveExtensions.IPFS_COMPANION,
                        BraveExtensions.IPFS_COMPANION_BETA,
                        BraveExtensions.WEB_TORRENT,
                    ]
                },
            ],
            "sockets.udp": [
                {
                    "dependencies": ["manifest:sockets"],
                    "contexts": ["blessed_extension"],
                    "allowlist": [
                        BraveExtensions.IPFS_COMPANION,
                        BraveExtensions.IPFS_COMPANION_BETA,
                        BraveExtensions.WEB_TORRENT,
                    ]
                },
            ],
        },
        "extensions/common/api/_manifest_features.json": {
            "sockets": [{
                "channel": "stable",
                "extension_types": ["extension", "platform_app"],
                "allowlist": [
                    BraveExtensions.IPFS_COMPANION,
                    BraveExtensions.IPFS_COMPANION_BETA,
                    BraveExtensions.WEB_TORRENT,
                ]
            }],
        },
        "extensions/common/api/_permission_features.json": {
            "hid": [
                {
                    "channel": "stable",
                    "extension_types": ["extension"],
                    "allowlist": [
                        BraveExtensions.ETHEREUM_REMOTE_CLIENT,
                    ]
                },
            ],
            "usbDevices": [
                {
                    "channel": "stable",
                    "extension_types": ["extension"],
                    "allowlist": [
                        BraveExtensions.ETHEREUM_REMOTE_CLIENT,
                    ]
                },
            ],
        },
    }

    for file_name in feature_extensions:
        EnsureFileIsBraveKnown(file_name)

    return feature_extensions.get(source_file, None)


@override_utils.override_method(FeatureCompiler)
def Load(self, original_method):
    original_method(self)

    PARENT_DIR_PREFIX = "../../"
    for source_file in self._source_files:
        assert source_file.startswith(PARENT_DIR_PREFIX)
        source_file = source_file[len(PARENT_DIR_PREFIX):]
        if source_file.startswith("brave/") or "/test/" in source_file:
            continue

        EnsureFileIsBraveKnown(source_file)

        feature_extensions = GetBraveFeatureExtensions(source_file)
        if feature_extensions:
            for name, features in feature_extensions.items():
                assert isinstance(features, list)

                existing_features = self._json.get(name, None)
                if existing_features is None:
                    raise RuntimeError(
                        f"Feature {name} not found in {source_file}")

                if not isinstance(existing_features, list):
                    existing_features = [existing_features]
                existing_features.extend(features)

                self._json[name] = existing_features
