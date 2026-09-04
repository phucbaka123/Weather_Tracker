# ESP32 Weather Tracker

A simple weather station built with an ESP32, a DHT11 temperature/humidity sensor, and a 16x2 I2C LCD. A push button cycles through three display modes: live readings, min/max history, and a comfort-level indicator.

## Features

- Live temperature (°C) and humidity (%) readings, updated every 2 seconds
- Min/max tracking — remembers the lowest and highest temp/humidity seen since power-on
- Comfort indicator — classifies current conditions as Cold / Ideal / Hot and Dry / Ideal / Humid
- Single-button interface to cycle between the three display modes
- Non-blocking sensor reads using `millis()` timing, so button presses stay responsive
- Debounced button input using edge detection (press-triggered, not held-triggered)

## Hardware

| Component | Notes |
|---|---|
| ESP32 dev board | Any standard ESP32 dev module |
| DHT11 sensor | 3-pin module (VCC, GND, Data) |
| 16x2 LCD with I2C backpack | 4-pin (GND, VCC, SDA, SCL) |
| Push button | 4-pin tactile button |
| Breadboard + jumper wires | |

### Wiring

**DHT11**
| DHT11 | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| DATA | GPIO 4 |

**16x2 LCD (I2C)**
| LCD | ESP32 |
|---|---|
| GND | GND |
| VCC | 5V |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

> I2C address may vary by unit — run an I2C scanner sketch to confirm (commonly `0x27` or `0x3F`).

**Push Button**
| Button | ESP32 |
|---|---|
| One side | GPIO 5 |
| Other side | GND |

Uses the ESP32's internal pull-up resistor (`INPUT_PULLUP`), so no external resistor is needed.

## Software

Built with [PlatformIO](https://platformio.org/) using the Arduino framework.

### Dependencies (`platformio.ini`)

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps =
    adafruit/DHT sensor library@^1.4.4
    adafruit/Adafruit Unified Sensor@^1.1.9
    marcoschwartz/LiquidCrystal_I2C@^1.1.4
```

## Getting Started

1. Clone this repository
2. Open the project folder in VS Code with the PlatformIO extension installed
3. Wire up the hardware as described above
4. Build and upload:
   ```
   pio run --target upload
   ```
5. Open the serial monitor to view live readings:
   ```
   pio device monitor
   ```

## Usage

- On boot, the LCD briefly displays "Weather Tracker," then switches to live readings
- Press the button to cycle through display modes:
  1. **Live readings** — current temperature and humidity
  2. **Min/Max** — lowest and highest values recorded since power-on
  3. **Comfort level** — Cold/Ideal/Hot and Dry/Ideal/Humid classification

## Roadmap

- [ ] Traffic-light module (Red/Yellow/Green) for at-a-glance comfort status
- [ ] WiFi web dashboard for remote viewing
- [ ] Cloud logging (ThingSpeak / Google Sheets) for historical graphs
- [ ] SD card data logging

## License

MIT (or update to your preferred license)
