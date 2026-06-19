# Fujitsu AC Remote for Flipper Zero

An infrared remote control application for Flipper Zero to control a **Fujitsu ASU9RLF** air conditioner unit, using the **AR-RAH1U** (AR-REB1E protocol) remote control profile.

## Features
- **Power Toggling**: Toggles power state between ON and OFF on the device, displaying the current active state ("ON" / "OFF") on-screen.
- **Fahrenheit Temperature Scale**: Supports temperature stepping by 2°F between **60°F and 88°F** (internally converted and checksummed as Celsius states for the Fujitsu IR protocol).
- **HVAC Modes**: Heat, Cool, Dry, and Auto.
- **Fan Speeds**: Auto, Quiet, Low, Medium, and High.
- **Vertical Swing**: Toggle vertical swing motion or lock it to a static position.

## Project Structure
- `lib/hvac_fujitsu/`: The core HVAC communication library containing timings, state generation, checksum computations, and mappings.
- `scenes/`: Application state scenes (loaded from settings format).
- `views/`: Panel view layouts.
- `assets/`: Pixel graphic assets (includes a custom Fahrenheit indicator border frame).

---

## Building and Installing

### 1. Building Standalone with `ufbt`
If you are developing against the standard Flipper firmware channel:
```shell
# Build the application
ufbt build

# Deploy and run on a connected Flipper Zero
ufbt launch
```

### 2. Building inside Flipper Firmware tree with `fbt`
If your Flipper is running a custom firmware:
1. Copy this folder to the `applications_user` directory in your Flipper firmware repository:
   ```shell
   cp -R flipperzero-fujitsu-ac-remote /path/to/flipperzero-firmware/applications_user/fujitsu_ac_remote
   ```
2. Navigate to your firmware repository root and run:
   ```shell
   ./fbt launch APPSRC=fujitsu_ac_remote
   ```
