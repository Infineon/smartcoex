# Smart Coex Library

## Overview

Bluetooth Low Energy (BLE) and Wi-Fi operate in the same band, and in some devices, share a single radio. This can cause data transmission on one interface to interfere with the other. Such interference can impact the performance of both Wi-Fi and BLE. In order to avoid such interference, coexistence (coex) configurations and algorithms are introduced in the underlying WLAN and BT stacks. These configurations can be tuned from the application to test and improve the performance of underlying Wi-Fi and BLE traffic when they are operated simultaneously.

This library provides an API that you can use to configure the coex parameters for WLAN and BLE.

## Features

Supports Wi-Fi BT coex configuration based on BLE Scan priority. You can set the LE Scan priority, with one of three options - high, medium, and low. Based on the priority, the different LE scan parameters are configured.

## Supported Platform(s)

### AnyCloud

- [PSoC 62S2 Wi-Fi BT Pioneer Kit (CY8CKIT-062S2-43012)](https://www.cypress.com/documentation/development-kitsboards/psoc-62s2-wi-fi-bt-pioneer-kit-cy8ckit-062s2-43012)

## Supported Framework(s)

This library supports the following framework(s):

- **AnyCloud Framework:** AnyCloud is a FreeRTOS-based solution. Smart Coex Library uses the [wifi-connection-manager](https://github.com/cypresssemiconductorco/wifi-connection-manager) library for Wi-Fi network connectivity functions, and [bluetooth-freertos](https://github.com/cypresssemiconductorco/bluetooth-freertos) for BLE.

## Dependencies

### AnyCloud

- [Wi-Fi Connection Manager](https://github.com/cypresssemiconductorco/wifi-connection-manager)

- [Connectivity Utilities Library](https://github.com/cypresssemiconductorco/connectivity-utilities)

- [bluetooth-freertos](https://github.com/cypresssemiconductorco/bluetooth-freertos)


## Quick Start

### AnyCloud

A set of pre-defined configuration files have been bundled with the wifi-mw-core library for FreeRTOS, lwIP, and mbedTLS.

Review the configuration and make the required adjustments. See the "Quick Start" section in [README.md](https://github.com/cypresssemiconductorco/wifi-mw-core/blob/master/README.md).

1. Define the following COMPONENTS in the application's Makefile for the Smart Coex Library. For additional information, see the "Quick Start" section in [README.md](https://github.com/cypresssemiconductorco/wifi-mw-core/blob/master/README.md).

   ```
   COMPONENTS=FREERTOS MBEDTLS LWIP WCM WICED_BLE
   ```

2. Add the following compiler directives to the `DEFINES` section in the application's Makefile:

   ```
   DEFINES=CYBSP_WIFI_CAPABLE
   ```

3. By default, log messages are disabled by the Smart Coex library. To enable log messages, the application must perform the following:

   - Add the `ENABLE_SMARTCOEX_LOGS` macro to the `DEFINES` in the code example's Makefile. The Makefile entry would look like this:

     ```
     DEFINES+=ENABLE_SMARTCOEX_LOGS
     ```
   - Call the `cy_log_init()` function provided by the *cy-log* module. cy-log is part of the *connectivity-utilities* library.

     See [connectivity-utilities library API documentation](https://cypresssemiconductorco.github.io/connectivity-utilities/api_reference_manual/html/group__logging__utils.html) for cy-log details.


## More Information

- [Smart Coex RELEASE.md](./RELEASE.md)

- [Smart Coex API Reference Guide](https://cypresssemiconductorco.github.io/smartcoex/api_reference_manual/html/index.html)

- [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.cypress.com/products/modustoolbox-software-environment)

- [ModusToolbox Device Configurator Tool Guide](https://www.cypress.com/ModusToolboxDeviceConfig)

- [PSoC 6 Technical Reference Manual](https://www.cypress.com/documentation/technical-reference-manuals/psoc-6-mcu-psoc-63-ble-architecture-technical-reference)

- [PSoC 63 with BLE Datasheet Programmable System-on-Chip datasheet](http://www.cypress.com/ds218787)
