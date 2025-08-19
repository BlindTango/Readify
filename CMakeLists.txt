cmake_minimum_required(VERSION 3.16)
project(readify)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets with required components
find_package(wxWidgets REQUIRED COMPONENTS core base html)

# Include wxWidgets directories
include(${wxWidgets_USE_FILE})

# Create the executable
add_executable(readify main.cpp)

# Link wxWidgets libraries
target_link_libraries(readify ${wxWidgets_LIBRARIES})

# Set executable properties for Windows
if(WIN32)
    set_target_properties(readify PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Copy assets folder to build directory
file(GLOB_RECURSE ASSET_FILES "assets/*")
if(ASSET_FILES)
    # Create assets directory in build output
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)
    
    # Copy all files from assets to build directory
    add_custom_command(TARGET readify POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
        COMMENT "Copying assets to build directory"
    )
    
    # For multi-config generators (like Visual Studio), also copy to config-specific directory
    if(CMAKE_CONFIGURATION_TYPES)
        add_custom_command(TARGET readify POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/$<CONFIG>/assets
            COMMENT "Copying assets to config-specific build directory"
        )
    endif()
endif()

# Install target
install(TARGETS readify DESTINATION .)
install(DIRECTORY assets DESTINATION . OPTIONAL)