# Match-3 on M5StickC-Plus (ESP32)

A tilt-controlled Match-3 / Bejeweled-style game written in C++ for the
**M5StickC-Plus** (ESP32) microcontroller. The board is steered by the
device's built-in IMU — you tilt the stick to move the cursor — and special
gems clear whole rows or columns when matched.

> **Status:** Working firmware, runs on real hardware. Built with PlatformIO.

<!-- TODO: add a short GIF of gameplay and a photo of the game running on the
     physical M5StickC-Plus. A 5-second clip here is worth more than any
     paragraph below. Drop them in a /docs folder and reference them here:
     ![gameplay](docs/gameplay.gif)  ![hardware](docs/hardware.jpg) -->

## What makes it interesting

- **Runs in a tiny resource envelope.** Fixed-size 9×9 buffers, no dynamic
  allocation — deliberate memory discipline for a constrained MCU.
- **Bit-packed game state.** Each cell is a single `int8_t`: bits 0–2 store the
  gem colour, bits 3–4 store the gem type (normal / row-clear / col-clear).
  See `include/config.h`.
- **Modular architecture** split across focused translation units:
  `board`, `match`, `render`, `input`, `menu`, `save`, `game_state`.
- **Match detection + cascades**, plus special-gem mechanics that clear an
  entire row or column.
- **Persistent state** via the ESP32 EEPROM emulation.

## Controls

| Input | Action |
|-------|--------|
| Tilt the device | Move the cursor across the board |
| Button A | Rotate the selection axis (horizontal ↔ vertical) |
| Button B | Commit the swap / continue from the end screen |
| Hold A + B | Open the in-game menu |

## Architecture

```
src/main.cpp          setup() + loop(): input → game logic → render
include/ + src/
  game_state.*         central GameState struct (the single source of truth)
  board.*              board generation, cursor, swaps
  match.*              match detection, clears, cascades, scoring
  render.*             drawing to the M5GFX framebuffer
  input.*              IMU tilt + button handling
  menu.*               in-game menu
  save.*               EEPROM persistence
  config.h             tunables + bit-packing helpers (gem colour/type)
```

## Hardware & dependencies

- **Board:** M5StickC-Plus (ESP32-PICO)
- **Framework:** Arduino (via PlatformIO)
- **Libraries:** `m5stack/M5GFX`, `m5stack/M5Unified` (pulled automatically by
  PlatformIO — see `platformio.ini`)

## Build & flash

```bash
# Install PlatformIO Core (https://platformio.org/install/cli), then:
pio run                 # build
pio run --target upload # flash to a connected M5StickC-Plus
pio device monitor      # serial monitor @ 115200 baud
```

PlatformIO downloads the M5GFX / M5Unified dependencies on first build, so no
libraries are vendored in this repo.

## What I learned / would do differently

<!-- TODO (recruiters love this section — keep it honest, 3–5 bullets):
  - e.g. "First time fitting game state into a single byte per cell."
  - e.g. "Would add unit tests for the match-detection logic next."
  - e.g. "Tilt thresholds needed tuning to feel responsive without drift." -->

## License

MIT — see [LICENSE](LICENSE).
