# Flipper Zero Application Developer Notes

This document contains references for managing local SDK environments and device configuration for the Fujitsu AC Remote application.

---

## 1. SDK Mismatch Troubleshooting (`ApiTooNew` / `ApiTooOld`)

When compiling and deploying the application standalone using `ufbt launch` on a Flipper Zero running custom firmware, you might encounter an error like:
`[ERROR] Unexpected response: Preload failed, ...: Update Firmware to use with this Application (ApiTooNew)`

### Why this happens
The Flipper Zero firmware validates that the API version of the compiled `.fap` matches the API version exposed by the operating system. If the SDK version used by `ufbt` is newer (`ApiTooNew`) or older (`ApiTooOld`) than the target device's firmware API, the FAP loader will refuse to load the app.

### How to resolve
To compile the app against the exact API version of your custom firmware, point `ufbt` to the SDK zip built directly from your local firmware repository:

1. **Package the local SDK zip**:
   In your firmware repository root directory (e.g. `flipperzero-firmware`), run:
   ```shell
   ./fbt fw_dist
   ```
   This compiles the firmware and packages the matching SDK at:
   `flipperzero-firmware/dist/f7-D/flipper-z-f7-sdk-local.zip`

2. **Point `ufbt` to the local SDK package**:
   In the application directory, configure `ufbt` by specifying the hardware target (`f7`) and the path to the zip package:
   ```shell
   ufbt update --hw-target f7 --local <path_to_flipperzero-firmware>/dist/f7-D/flipper-z-f7-sdk-local.zip
   ```

3. **Clean cached build files**:
   Clean existing build files to ensure they compile with the newly loaded SDK:
   ```shell
   ufbt -c
   ```

4. **Launch the application**:
   ```shell
   ufbt launch
   ```

---

## 2. Adding the Application to Favorites

Favorite files and applications on the Flipper Zero are loaded from a list of paths stored in `/ext/favorites.txt` on the SD card.

### Method 1: Using the Flipper Zero Interface (GUI)
1. On the Flipper home screen, press the **Right** button to open the **Archive** menu.
2. Navigate to the **Apps** tab.
3. Browse to the **Infrared** folder and highlight `fujitsu_ac_remote.fap`.
4. Press and hold the **OK** button to open the context menu.
5. Select **Pin**. 
   * *The application will now appear under the **Favorites** tab in the Archive app.*

### Method 2: Manually Editing `favorites.txt`
You can edit the `/ext/favorites.txt` file on the SD card directly (using qFlipper's file manager or a card reader) and append the path to the application:
```text
/ext/apps/Infrared/fujitsu_ac_remote.fap
```
