/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

import * as leo from '@brave/leo/tokens/css'

import * as mixins from '../../shared/lib/css_mixins'

export const root = styled.div`
  position: fixed;
  top: 0;
  left: 0;
  bottom: 0;
  right: 0;
  font-family: Poppins;
  background: ${leo.color.container.background};
  overflow: auto;
  padding: 32px 30px;
  display: flex;
  flex-direction: column;
  align-items: center;

  a {
    color: ${leo.color.text.interactive};
    text-decoration: underline;
  }
`

export const branding = styled.div`
  width: 100%;
  display: flex;
  gap: 2px;
  align-items: center;

  .logo {
    --leo-icon-size: 36px;
  }

  .logo-text .icon {
    display: block;
    height: 18px;
    width: auto;
  }
`

export const content = styled.div`
  max-width: 607px;
`

export const nav = styled.div`
  margin-top: 8px;
`

export const title = styled.div`
  color: ${leo.color.text.primary};
  font-size: 28px;
  font-weight: 500;
  line-height: 40px;
  margin-top: 32px;
  padding: 8px 0;
  text-align: center;
`

export const text = styled.div`
  color: ${leo.color.text.secondary};
  font-size: 14px;
  font-weight: 400;
  line-height: 24px;
  padding: 8px 64px;

  .layout-narrow & {
    padding: 8px 0;
  }
`

export const providerSelection = styled.div`
  padding: 32px 64px;

  .layout-narrow & {
    padding: 8px 0;
  }
`

export const providerGroupHeader = styled.div`
  color: ${leo.color.text.secondary};
  font-size: 14px;
  font-weight: 400;
  line-height: 24px;
`

export const providerGroupHeaderIcon = styled.span`
  color: ${leo.color.icon.default};

  --leo-icon-size: 20px;

  leo-icon {
    display: inline-block;
    vertical-align: text-bottom;
    margin-left: 8px;
  }
`

export const providerGroup = styled.div`
  margin-top: 8px;
  border-radius: 8px;
  border: 1px solid ${leo.color.divider.subtle};
  display: flex;
  flex-direction: column;
  padding: 0 24px;

  button {
    ${mixins.buttonReset};
    padding: 16px 0;
    cursor: pointer;
    display: flex;
    align-items: center;
    gap: 11px;
    text-align: left;
    border-top: solid 1px ${leo.color.divider.subtle};

    &:first-child {
      border-top: none;
    }

    &:disabled {
      cursor: default;
    }
  }
`

export const providerButtonIcon = styled.div`
  padding: 10px;

  .icon {
    height: 32px;
  }
`

export const providerButtonName = styled.div`
  flex: 1 1 auto;
  color: ${leo.color.text.primary};
  font-size: 16px;
  font-weight: 600;
  line-height: 24px;
`

export const providerButtonMessage = styled.div`
  color: ${leo.color.text.secondary};
  font-size: 12px;
  font-weight: 400;
  line-height: 18px;
`

export const providerButtonCaret = styled.div`
  color: ${leo.color.icon.interactive};
  --leo-icon-size: 24px;
`

export const regionsLearnMore = styled.div`
  margin-top: 8px;
  font-size: 12px;
  font-weight: 400;
  line-height: 18px;
  letter-spacing: 0.12px;
`
