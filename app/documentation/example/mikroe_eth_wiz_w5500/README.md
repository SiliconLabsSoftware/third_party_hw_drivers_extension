# W5500 - ETH WIZ Click (Mikroe) #

## Summary ##

This project aims to implement a hardware driver for the W5500 ethernet controller that is integrated on the ETH Wiz click board. The W5500 chip is a Hardwired TCP/IP embedded Ethernet controller that enables easier internet connection for embedded systems using SPI (Serial Peripheral Interface).

W5500 enables users to have the Internet connectivity in their applications just by using the single chip to implement TCP/IP Stack, 10/100 Ethernet MAC and PHY. Hardwired TCP/IP stack supports TCP, UDP, IPv4, ICMP, ARP, IGMP, and PPPoE. W5500 uses a 32Kbytes internal buffer as its data communication memory. For more information about the W5500, see the following [source](https://docs.wiznet.io/img/products/w5500/w5500_ds_v109e.pdf).

The board can be used for industrial automation systems, IP set-top boxes, VoIP/Video phone systems, security systems, home networks and gateways and test and measurement equipment and for many other applications.

## Required Hardware ##

- 1x [BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit) BGM220 Bluetooth Module Explorer Kit
- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))
- 1x [ETH WIZ Click board](https://www.mikroe.com/eth-wiz-click)
- 1x Ethernet Cable, e.g. [Ethernet Roll Cable](https://www.mikroe.com/ethernet-roll-transparent)

## Hardware Connection ##

- **If the BGM220P Explorer Kit is used:**

  The ETH WIZ Click board supports MikroBus; therefore, it can easily connect to the MikroBus socket of the BGM220P Explorer Kit. Be sure that the 45-degree corner of the board matches the 45-degree white line of the Explorer Kit. The hardware connection is shown in the image below:

  ![board](image/board.png)

- **If the Wi-Fi Development Kit is used**:

  The hardware connection is shown in the table below:

  | Description  | BRD4338A + BRD4002A | BRD2605A     | ETH WIZ Click       |
  | -------------------------| ------------- | -------------------- | ------------------ |
  | GPIO_RESET               | GPIO_47 [P26]       | GPIO_11        | RESET              |
  | GPIO_CS                  | GPIO_46 [P24]       | GPIO_10        | CS                 |
  | RTE_GSPI_MASTER_CLK_PIN  | GPIO_25 [P25]       | GPIO_25        | SCK                |
  | RTE_GSPI_MASTER_MISO_PIN | GPIO_26 [P27]       | GPIO_26        | SDO                |
  | RTE_GSPI_MASTER_MOSI_PIN | GPIO_27 [P29]       | GPIO_27        | SDI                |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add the your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by "w5500".

2. Click **Create** button on the **Third Party Hardware Drivers - W5500 - ETH Wiz Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_eth_wiz_w5500/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

   - Open the .slcp file in the project.

   - Select the *SOFTWARE COMPONENTS* tab.

   - Install the following components:

      **If the BGM220P Explorer Kit is used:**
       - [Services] → [IO Stream] → [IO Stream: EUSART] with the default instance name: **vcom**
       - [Application] → [Utility] → [Log]
       - [Application] → [Utility] → [Assert]
       - [Platform] → [Driver] → [SPI] → [SPIDRV] → [mikroe] → change the configuration for [SPI master chip select (CS) control scheme] to "CS controlled by the application" and select the CS pin to None as below:

           ![spi config](image/spi_config.png)

       - [Third Party Hardware Drivers] → [Interface] → [W5500 - ETH WIZ Click (Mikroe)] → use the default configuration

          ![config](image/w5500_config.png)

      **If the Wi-Fi Development Kit is used:**
       - [Third Party Hardware Drivers] → [Interface] → [W5500 - ETH WIZ Click (Mikroe)] → use the default configuration
       - [Application] → [Utility] → [Assert]

4. Build and flash the project to your device.

## How It Works ##

### API Overview ###

A detailed description of each function can be found in doc/doxygen.

The driver is divided into an Application layer, a TCP/IP stack and a Physical layer.
The Application layer is where applications requiring network communications live. Examples of these applications include email clients and web browsers. These applications use the TCP/IP stack to send requests to connect to remote hosts.

The TCP/IP stack establishes the connection between applications running on different hosts. It uses TCP for reliable connections and UDP for fast connections. It keeps track of the processes running in the applications above it by assigning port numbers to them and uses the Network layer to access the TCP/IP network.

The physical layer provides integration to the host microcontroller hardware-dependent codes.

![software_layers](image/software_layers.png)

`dhcp.c`: DHCP library. The dynamic host configuration protocol (DHCP) is the application responsible for requesting and offering IP addresses.

`dns.c`: DNS library. A Domain Name System (DNS) enables to browse a website by providing the website or domain name instead of the website’s IP address.

`sntp.c`: SNTP library. SNTP stands for Simple Network Time Protocol, which is a service that provides the time of day to network devices. Typical accuracy is in the range of hundreds of milliseconds.

`http_server.c`: HTTP server library. The Hypertext Transfer Protocol (HTTP) is the most commonly used TCP/IP application as it transfers web pages from a web server to a web browser.

`socket.c`: Enables applications to connect to a Transmission Control Protocol/Internet Protocol (TCP/IP) network.

`ethernet_udp.c`: Library to send/receive UDP packets.

`ethernet_server.c`: Library is for all Ethernet server based calls. It is not called directly but invoked whenever you use a function that relies on it.

`ethernet_client.c`: Library is for all Ethernet client based calls. It is not called directly but invoked whenever you use a function that relies on it.

`w5x00.c`: Implements public interfaces to interact with the W5500 chip.

`w5x00_platform.c`: Integrates the Silabs SPI driver for SPI communication.

### Testing ###

This example demonstrates the HTTP client features of the driver.
The connection diagram of this example is shown in the image below:

![diagram](image/diagram.png)

The following diagram shows the program flow as implemented in the file `app.c`:

![Work Flow](image/flow.png)

Use a terminal program, such as the Console that is integrated in Simplicity Studio or a third-party tool terminal like PuTTY to receive the logs from the virtual COM port. Note that the board uses the default baud rate of 115200. You should expect a similar output to the one below.

![log](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
