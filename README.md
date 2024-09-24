# Photoshop Plugin Template

This repository provides a starting point for developers to create a C++ Photoshop plugin from scratch. This README will guide you through setting up your development environment, understanding dependencies, and getting your plugin up and running.

## Features

Currently, this template works only for Windows.

The build pipeline includes:
- `pipl.rr`
- `pipl.rc`
- `pipl.res`
- Plugin

## Requirements

- [Adobe Photoshop SDK 2024 (Windows) v1](https://developer.adobe.com/console/servicesandapis)
- [MSVC v141 - Visual Studio 2017 C++ x64/x86 Build Tools (v14.16)](https://visualstudio.microsoft.com/downloads/)

> **Note**: Even if you install Visual Studio Build Tools 2019, make sure to select **MSVC v141** under the optional components panel. If you already have Visual Studio Build Tools installed, you can modify the installation using the Visual Studio Installer.

## Using This Template

To start your own Photoshop plugin development, follow these steps to use this repository as a template:

1. Click the **"Use this template"** button at the top right of this repository’s GitHub page.
2. Create a new repository with a unique name, or add it to an existing repository.
3. Once the repository is created, you can start customizing the plugin to suit your needs.

### Cloning the Repository

After creating your new repository from the template, clone it to your local machine:

```bash
git clone https://github.com/your-username/your-repository-name.git
cd your-repository-name
```

Make sure to replace `your-username` and `your-repository-name` with your actual GitHub username and repository name.

## Initial Setup

1. Edit the variable section of the `CMakeLists.txt` file with your plugin information:

    ```cmake
    project(
        "YourPluginName"                        # Plugin Name
        VERSION 1.0.0                           # Plugin Version
        DESCRIPTION "Your plugin description"   # Plugin Description
    )
    ```

2. Set the plugin category and UUID in the `CMakeLists.txt`:

    ```cmake
    set(PLUGIN_CATEGORY "Your Plugin Category")
    set(UUID_NAMESPACE "your-plugin-uuid-goes-here")
    ```
   You can create your own UUID using any UUID generator like [UUIDGenerator.net](https://www.uuidgenerator.net).

3. Download the latest Photoshop SDK from [Adobe's developers site]((https://developer.adobe.com/console/servicesandapis)), decompress it, and place the SDK directory at the root of the project (at the same level as this `README`).

4. Ensure the SDK directory version in `CMakeLists.txt` matches the SDK version you downloaded:

    ```cmake
    set(PS_SDK_VERSION "adobe_photoshop_sdk_2024_win_v1")
    ```

5. Modify the following constants in `define.h` with your plugin-specific details:

    ```c++
    #define SUITE_ID 'pspg'
    #define EVENT_ID 'PsPg'
    #define VENDOR_NAME "Your Vendor Name"
    ```

## Building and Deployment

1. Ensure that you are using **MSVC v141** as the compiler. Other compilers might not work properly.

2. Build the project using CMake or Visual Studio:
   - Open the project in Visual Studio or run CMake to generate the build files.
   - Build the plugin in **Release** mode for deployment.

3. Once the build completes, the plugin `.8bf` file will be generated. This file is the plugin that can be loaded into Photoshop.

### Running the Plugin in Photoshop

1. Copy the built `.8bf` plugin file to the Photoshop plugin directory:
   - **Default location**: `C:\Program Files\Adobe\Adobe Photoshop 2024\Plug-ins\`

2. Launch Photoshop, and the plugin should appear under the **Plugins** menu.

## Start Developing

The initial setup generates a plugin with the `.8bf` extension, which is the general file type for Photoshop plugins. If you change the plugin type, make sure to modify the `pipl.r` file accordingly.

For more information on different plugin types and configuration, refer to the Photoshop SDK documentation:  

- [Plugin Types](adobe_photoshop_sdk_2024_win_v1/pluginsdk/documentation/html/pgplugintypes.html).
- [API Guide](adobe_photoshop_sdk_2024_win_v1/pluginsdk/documentation/Photoshop%20API%20Guide.pdf).
- [PICA](adobe_photoshop_sdk_2024_win_v1/pluginsdk/documentation/PICA.pdf).
- [General Documentation](adobe_photoshop_sdk_2024_win_v1/pluginsdk/documentation.html).

## License

This plugin template is licensed under the [CC0 1.0 Universal](LICENSE).