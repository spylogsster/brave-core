/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import { BrowserRouter } from 'react-router-dom'
import { createGlobalStyle } from 'styled-components'

import { setIconBasePath } from '@brave/leo/react/icon'
import '@brave/leo/tokens/css/variables.css'

// Components
import App from './components/app.v1'

// Theme

import wireApiEventsToStore from './apiEventsToStore'
import startReceivingPlayerEvents from './playerEventSink'

// Utils
import store from './store'

const GlobalStyle = createGlobalStyle`
  #root { 
    height: 100%; 
  }
  body {
    margin: 0;
    background-color: var(--leo-color-container-background);
  }
`

function initialize () {
  render(
    <>
      <GlobalStyle />
      <BrowserRouter>
        <Provider store={store}>
          <App />
        </Provider>
      </BrowserRouter>
    </>,
    document.getElementById('root')
  )
}

wireApiEventsToStore()
startReceivingPlayerEvents()
setIconBasePath('chrome-untrusted://resources/brave-icons')

document.addEventListener('DOMContentLoaded', initialize)
