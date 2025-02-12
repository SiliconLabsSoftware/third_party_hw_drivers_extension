# Third Party Hardware Drivers

## Adding SDK Extensions for Hardware Drivers

You can find the Third Party Hardware Drivers GSDK Extension at Github, as a first step you should clone the repository to your local computer.

You can follow this step-by-step guide to install and use the extension or watch a guideline video showing the same steps.

[![GuidelineVideo](doc/studio/sdke/sdke-demo-video.png)](https://www.brainshark.com/siliconlabs/Third-Party-Hardware-Drivers)

* **STEP 1** Clone Third Party Hardware Drivers repository from Github

  It is up to you to choose a folder on your computer to clone the repository into.

  > git clone https://github.com/SiliconLabs/third_party_hw_drivers_extension.git

* **STEP 2** Open Simplicity Studio and go to Window/Preferences

  ![Preferences](doc/studio/sdke/preferences.png)

* **STEP 3** Go Simplicity Studio/SDKs, select an installed GSDK (version >=4.2.0), click on **Add Extension**

  ![AddSDKExtension](doc/studio/sdke/sdks.png)

* **STEP 4** Select the SDK Extension's location, click on **OK**

  The SDK Extension will be detected in the repository folder.

  ![SelectExtension](doc/studio/sdke/add-extension.png)

* **STEP 5** Click on the **Trust** button on the Verify SDK Extension dialog

  ![TrustSDKExtension](doc/studio/sdke/sdke-trust.png)

* **STEP 6** The SDK Extension successfully installed, click on **Apply and Close**

  ![Installed](doc/studio/sdke/installed.png)

### Example Project Templates

The Third Party Hardware Drivers extension provides example project templates for each supported driver.

* **STEP 1** Open the Launcher perspective in Simplicity Studio

  ![LauncherPerspective](doc/studio/sdke/launcher.png)

* **STEP 2** Select a product either in the My Products or in the Debug Adapters dialog

* **STEP 3** Filter examples by typing "third" or "Third Party" in the "filter on keywords" input
* **STEP 4** Select an example project from the resources and click on the **Create** button.

  ![LauncherCreateProject](doc/studio/sdke/launcher-examples.png)

* **STEP 5** Follow the steps in the New Project Wizard dialog.

Launcher creates a new project based on the selected template, this project contains basic example on how your application can integrate a driver using the extension.

### Software Components

* **STEP 1** Open a project configuration (The selected perspective should be "Simplicity IDE".).

* **STEP 2** Select **SOFTWARE COMPONENTS** and search with "third" keyword in the Search keywords, component's name input

  > Make sure that the components with **Evaluation** quality level are enabled in the Software Components view.

  ![SoftwareComponents](doc/studio/sdke/sw-components.png)

* **STEP 3** Select a driver from the list by clicking on it, click on the **Install**

  ![SoftwareComponentsInstall](doc/studio/sdke/sw-components-installed.png)

The selected hardware driver is installed to your project. The installed driver's API can be found in the extension's public folder.

  ![PublicAPI](doc/studio/sdke/driver-api.png)

Public header files should be included in your application for each installed driver.

For further information on how to use drivers in your project see [application example templates](#example-project-templates) in the Launcher.
