cmake_minimum_required(VERSION 3.16)
project(Readify)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base webview)

# Include wxWidgets
include(${wxWidgets_USE_FILE})

# Create source directory
file(GLOB SOURCES 
    "src/*.cpp"
    "src/*.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link wxWidgets
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# For Windows, set subsystem to windows to avoid console window
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()