# Readify
An application to read various document types.

## Overview
Readify is a cross-platform document reader application built with wxWidgets and C++. It provides a modern interface for reading and navigating through various document formats.

## Features
- **Modern GUI**: Clean interface with menubar, toolbar, and status bar
- **Document Navigation**: Tree-based document structure browser
- **Document Viewer**: Built-in browser control for rendering documents
- **Progress Navigation**: Percentage-based scrollbar for quick document navigation
- **Cross-platform**: Runs on Windows, Linux, and macOS

## Requirements
- C++17 compiler (GCC, Clang, or MSVC)
- CMake 3.16 or later
- wxWidgets 3.2 or later
- WebKit2GTK (Linux) or equivalent web engine

## Building on Linux (Ubuntu/Debian)

### Install Dependencies
```bash
sudo apt-get update
sudo apt-get install -y libwxgtk3.2-dev libwxgtk-webview3.2-dev build-essential cmake
```

### Build Application
```bash
mkdir build
cd build
cmake ..
make
```

### Run Application
```bash
./bin/Readify
```

## Application Components

### Main Interface
- **Menubar**: File operations, editing tools, view controls, and help
- **Toolbar**: Quick access to common actions with icons
- **Split Layout**: 
  - Left panel: Navigation tree for document structure
  - Right panel: Document viewer with web rendering capabilities
- **Status Bar**: Document position control with percentage-based slider

### Supported Features
- Document opening through File menu
- Copy/paste operations
- Zoom controls
- Tree-based navigation
- Progress tracking via status bar slider

## Architecture
The application follows a clean MVC pattern:
- `ReadifyApp`: Main application class
- `MainFrame`: Primary window with all UI components
- Event-driven architecture for user interactions
- Modular design for easy extension to support additional document formats
