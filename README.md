# Readify
An application to read various document types.

## Overview
Readify is a lightweight document reader built with wxWidgets that allows you to view HTML documents with ease. It features a clean interface with navigation capabilities and is designed for cross-platform compatibility.

## Features
- Clean and intuitive user interface
- HTML document viewing with wxHtmlWindow
- File navigation tree
- Toolbar with common actions
- Status bar for file information
- Cross-platform compatibility

## Download (Windows)
If you don't have a C++ compiler installed, you can download the pre-built Windows executable:

1. Go to the [Actions](https://github.com/BlindTango/Readify/actions) page
2. Click on the latest successful "Build Readify (Windows)" workflow run
3. Download the `readify-windows-x64` artifact
4. Extract the ZIP file and run `readify.exe`

The download includes the executable and sample HTML files in the `assets` folder.

## Building from Source
If you want to build Readify from source, you'll need:

### Prerequisites
- CMake 3.16 or later
- wxWidgets library (core, base, html components)
- C++17 compatible compiler

### Windows with vcpkg
```bash
# Install wxWidgets via vcpkg
vcpkg install wxwidgets:x64-windows

# Configure and build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

### Other Platforms
```bash
# Make sure wxWidgets is installed (package manager or from source)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Usage
1. Launch Readify
2. The application will automatically load `assets/index.html` if available
3. Use the File menu or toolbar to open HTML files
4. Navigate between documents using the tree view on the left
5. View HTML content in the main panel

## License
This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.
