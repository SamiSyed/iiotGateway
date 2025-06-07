# iiotGateway

This project uses STM32WL5Exx based controller.

Device - Seeedstudio E5 dev board https://www.seeedstudio.com/Wio-E5-LE-Dev-Kit-STM32WLE5JC-p-5763.html

Architecture :
GATEWAY <-- EndNode
Both using same hardware.

RTOS is being used.

Code is written on pingpong example. https://github.com/johflo/Seeed-Lora-e5/tree/main/Seeed-LoRa-E5_PingPong

Build is done using CMake

General structure

The repository is an STM32‑based IoT gateway project. The README explains that it targets the STM32WL5Exx (Wio‑E5 development board) and acts as a gateway receiving data from LoRa end nodes and forwarding to the cloud via MQTT.

Key directories:

Core/ – CubeMX‑generated application code. Inc/ has headers, Src/ contains the C source files.

Drivers/ – STM32 HAL/BSP and CMSIS libraries.

Middlewares/Third_Party/ – ST-provided LoRaWAN and SubGHz PHY stacks.

SubGHz_Phy/ – LoRa application layer used by the gateway (subghz_phy_app.c etc.).

Utilities/ – helper modules for tracing, low‑power management, timers and the simple task sequencer.

target/ – CMake toolchain file for arm-none-eabi.

Configuration/IDE files in .vscode/ and build scripts (Makefile, CMakeLists.txt). A helper script clangify formats the source with clang-format.

Important modules

System definitions
user_system.h declares system constants and types (sensor buffers, UART defines, MQTT message sizes, etc.) and exposes helper functions for LoRa/MQTT handling and UART state management.

LoRa communication
subghz_phy_app.c handles sending a request to an end node and parsing incoming LoRa messages. The processIncomingLoraMessage function splits the received string, extracts sensor values and updates global data structures before signalling that new data arrived.

MQTT messaging
user_mqttFunctions.c builds JSON strings containing sensor readings and sends them with AT commands to a GSM module (sendDataToMqttBroker → sendMqttServer). The function prepareMqttMessageStruct assembles the JSON payload with temperature, moisture, conductivity, pH, nitrogen, phosphorus and potassium values before transmitting.

Main loop
main.c initializes peripherals, sets up timers, LoRa and GSM, and then loops endlessly. It schedules LoRa polling, runs moving‑average filters on newly received data, periodically sends sensor readings via MQTT, checks timeouts and refreshes the watchdog.

HTML configuration
A simple web page (Core/Src/index.html) is converted to C via a custom command in CMakeLists.txt. The generated index_html.c is served by the minimal WiFi/HTTP module (user_wifi.c). The conversion process is defined around lines 158‑178 in CMakeLists.txt.

Code‑generation helpers
cubeMX_CodeGen_Postrun.py is a post‑processing script to remove the MX_SubGHz_Phy_Process() call from main.c after CubeMX generation. A batch file runPostGenScript.bat runs this script automatically.

Things to learn next

ST HAL and middleware – Understanding HAL drivers and the LoRa/SUBGHz stack will help modifying radio or peripheral configuration.

CMake build – target/gcc-arm-none-eabi.cmake sets up the Arm GCC toolchain. Study the build options and how CMakeLists.txt groups HAL, middleware and user sources.

UART communication – Examine UartRingbuffer_multi and ringBuffer modules for interrupt‑driven serial buffering if adding more serial devices.

System timing – The project uses ST’s lightweight sequencer (stm32_seq) and custom timing utilities in Utilities/. Learning these helps when adding tasks or timeouts.

Network protocols – Review AT command handling in user_system.c and MQTT packet creation in user_mqttFunctions.c if extending GSM/MQTT features.

HTML/UI – The gateway hosts a tiny web page converted to C; customizing index.html and rebuilding regenerates the embedded page.

Overall the codebase combines CubeMX‑generated HAL code with custom modules for LoRa communication, MQTT transmission, UART buffering and a small web server. Understanding how these layers interact—LoRa → filtering → MQTT via GSM, plus optional WiFi configuration—will make future development easier.
