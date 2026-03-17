## Embedded Web UI Pipeline

The Nuxt app can now be exported into `PROGMEM`-ready C++ assets for the ESP8266 firmware.

### Commands

From [webapp/package.json](../webapp/package.json):

```bash
npm run build:embedded
```

This does two things:

1. Runs `nuxt generate` with the current root routes.
2. Converts the generated static output into C++ asset files under `microcontroller/src/webui/`.

### Generated Files

The export script writes:

- `microcontroller/src/webui/generated_assets.h`
- `microcontroller/src/webui/generated_assets.cpp`

Each generated asset contains:

- the public route path
- the original source path from the Nuxt static output
- the content type
- gzipped file data stored in `PROGMEM`

### Current Status

This step only prepares the embedded asset bundle.
The firmware server is not yet wired to serve `EMBEDDED_ASSETS` instead of `LittleFS`.

### Next Step

Update the firmware server to:

1. look up requests inside `EMBEDDED_ASSETS`
2. respond with `request->send_P(...)`
3. set `Content-Encoding: gzip` for embedded files