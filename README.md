<div align="center">

# 💰 Bitcoin Ticker

### Forge a real-time Bitcoin display for ESP8266 and MAX7219.

Bitcoin Ticker is a two-part project for building a WiFi-connected Bitcoin display with an ESP8266 microcontroller, a MAX7219 LED matrix, and a Nuxt 4 web app for configuration.

<p>
  <a href="https://github.com/brumaombra/bitcoin-ticker"><img alt="GitHub Repo" src="https://img.shields.io/badge/github-brumaombra%2Fbitcoin--ticker-111111?logo=github"></a>
  <img alt="ESP8266" src="https://img.shields.io/badge/esp8266-ESP8266-E7352C?logo=espressif&logoColor=white">
  <img alt="PlatformIO" src="https://img.shields.io/badge/platformio-6.1.5-FF6B35?logo=platformio&logoColor=white">
  <img alt="Nuxt 4" src="https://img.shields.io/badge/nuxt-4-00DC82?logo=nuxt&logoColor=white">
  <img alt="License MIT" src="https://img.shields.io/badge/license-MIT-2563EB">
</p>

<p>
  📡 WiFi-connected ticker • 📺 MAX7219 LED matrix output • 🎛️ Web-based setup and settings • 🌍 EN/IT UI • ⚡ Lightweight Nuxt UI
</p>

<p>
  <a href="#features"><strong>Features</strong></a> •
  <a href="#architecture"><strong>Architecture</strong></a> •
  <a href="#quick-start"><strong>Quick Start</strong></a> •
  <a href="#firmware-overview"><strong>Firmware Overview</strong></a> •
  <a href="#web-app-overview"><strong>Web App Overview</strong></a> •
  <a href="#build-and-deploy"><strong>Build and Deploy</strong></a>
</p>

</div>

Bitcoin Ticker is designed for a simple hardware-to-web workflow: the ESP8266 fetches Bitcoin market data, drives the LED matrix, and exposes a local configuration interface that stays aligned with the firmware settings model.

<a id="features"></a>
## ✨ Features

- Live Bitcoin data on a MAX7219 LED matrix
- ESP8266 firmware with WiFi, flash-backed local storage, and embedded HTTP configuration
- Web-based configuration for WiFi credentials, API key, and display settings
- Adjustable scroll speed, selected metrics, and matrix behavior
- Nuxt 4 interface that mirrors the firmware configuration flow
- Reusable frontend UI primitives, shared dialogs, and confirmation flows
- English and Italian localization with browser language detection

<a id="architecture"></a>
## 🏗️ Architecture

Bitcoin Ticker is built around three cooperating parts:

- 🔌 **ESP8266 firmware** handles WiFi, data fetching, settings storage, and the embedded HTTP server
- 📺 **MAX7219 LED matrix** displays the live Bitcoin ticker output
- 🌐 **Nuxt web app** provides the browser-based setup and configuration interface with theme and language support

### 🔄 Data Flow

1. The web app sends configuration changes to the ESP8266 over HTTP.
2. The firmware stores settings and fetches Bitcoin market data from external APIs.
3. The matrix renders the current values as scrolling text.

<a id="quick-start"></a>
## 🚀 Quick Start

### 📋 Requirements

- ESP8266 board, such as NodeMCU
- MAX7219 LED matrix module
- Jumper wires and a stable power supply
- VS Code with PlatformIO installed
- A WiFi network and a Bitcoin market data API key

### 🔧 Initial Setup

1. Open the repository in VS Code.
2. Install the PlatformIO extension if it is not already installed.
3. Connect the ESP8266 and MAX7219 matrix according to the pinout used in the firmware.
4. Run `npm run build:embedded` from `webapp/` if you need to refresh the embedded web UI bundle.
5. Build and upload the firmware from the `microcontroller/` project.
6. Flash the device and connect to its setup access point when it starts for the first time.
7. Enter WiFi credentials and the API key through the web interface.

### 🔄 Configuration Flow

1. The device creates a temporary access point on first boot.
2. Connect to the AP and open the device IP address in a browser.
3. Save WiFi credentials so the ESP8266 can join your local network.
4. Save the API key used for market data requests.
5. Adjust display settings such as visible metrics and scroll speed.
6. Reboot or let the device refresh settings as needed.

<a id="firmware-overview"></a>
## 🔌 Firmware Overview

The firmware is responsible for the runtime behavior of the physical ticker.

- Connects the ESP8266 to WiFi
- Persists settings in flash-backed local storage
- Hosts the embedded HTTP server
- Fetches and parses Bitcoin market data
- Renders scrolling text to the MAX7219 matrix
- Serves the embedded web UI assets

Key firmware areas live under `microcontroller/src/`:

- `api/` for data fetching and parsing
- `config/` for settings and validation
- `matrix/` for LED output
- `server/` for HTTP routes
- `storage/` for EEPROM persistence
- `wifi/` for network connection logic

<a id="web-app-overview"></a>
## 🌐 Web App Overview

The Nuxt app provides the browser-based device configuration interface.

- Mirrors the firmware settings model
- Lets users configure WiFi and API credentials
- Controls display behavior and matrix preferences
- Supports English and Italian localization
- Includes confirm and message dialogs for critical actions
- Uses reusable UI primitives under `webapp/app/components/ui/`
- Shares device API helpers and theme utilities in `webapp/app/composables/`

Important web app areas:

- `webapp/app/pages/` for the main setup screens
- `webapp/app/components/ui/` for shared interface elements
- `webapp/app/composables/useDeviceApi.js` for device communication
- `webapp/app/composables/useUtils.js` for theme, dialog, and app helpers
- `webapp/i18n/locales/` for translations

<a id="build-and-deploy"></a>
## 🛠️ Build and Deploy

### 🔌 Firmware

- Run `npm run build:embedded` from `webapp/` when the embedded UI changes.
- Open `microcontroller/` in PlatformIO.
- Build and upload the firmware.

### 🌐 Web App

- Open `webapp/` in Nuxt.
- Install dependencies with `npm install`.
- Run `npm run dev` for local work.
- Run `npm run build` for a production static build.
- Run `npm run build:embedded` to export the embedded web UI bundle used by the firmware.

### 📦 Embedded Web UI

The repository includes an embedded UI export path so the web app can be packaged into firmware assets when needed.

- Source: `webapp/`
- Embedded docs: `docs/embedded-webui.md`
- Export script: `webapp/scripts/export-embedded-static.js`

## 📄 License

MIT