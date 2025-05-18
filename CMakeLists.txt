cmake_minimum_required(VERSION 3.18)

# This project builds both a native C++ shared library (mod_core)
# and a managed (.NET) assembly (FreeSwitch.Core) using CMake's CSharp support.
project(mod_core LANGUAGES CXX CSharp)

# Ensure multi-configuration is enabled if not already set.
if(NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release)
endif()

###############################################################################
# Native (C++) target: mod_core (shared library)
###############################################################################

# List native source files (adjust paths as required)
set(NATIVE_SOURCES
    ./mod_core.cpp
    ./freeswitch_core.cpp
    ./freeswitch_wrap.cxx
)

# Create the shared library for the native module.
add_library(mod_core SHARED ${NATIVE_SOURCES})

# Specify include directories for native code.
target_include_directories(mod_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(mod_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

# Define a preprocessor symbol for mod_core.
target_compile_definitions(mod_core PRIVATE -DMOD_CORE)

# Set compile options.
if(MSVC)
    target_compile_options(mod_core PRIVATE /W4 /EHsc)
else()
    target_compile_options(mod_core PRIVATE -Wall -Wextra -Werror)
endif()

###############################################################################
# Managed (C#) target: FreeSwitch.Core assembly
###############################################################################

# List managed (.cs) source files (adjust paths if needed)
set(CS_SOURCES
    core/AssemblyInfo.cs
    core/Extensions.cs
    core/Loader.cs
    core/Log.cs
    core/ManagedSession.cs
    core/PluginInterfaces.cs
    core/PluginManager.cs
    core/ScriptPluginManager.cs
    core/ChannelVariables.cs
    core/Util.cs
    core/swig.cs
    core/XmlSearchBinding.cs
)

# Create a managed (C#) library.
add_library(FreeSwitch.Core SHARED ${CS_SOURCES})

# Set Visual Studio-specific properties:
# Targeting .NET 5.0; adjust target framework version if necessary.
set_target_properties(FreeSwitch.Core PROPERTIES
    DOTNET_TARGET_FRAMEWORK "9.0"
    VS_DOTNET_OUTPUT_TYPE "Library"
    VS_DOTNET_REFERENCES "System;System.Core;Microsoft.CSharp;System.Xml.Linq;System.Data.DataSetExtensions;System.Data;System.Xml"
    VS_DOTNET_ROOTNAMESPACE "FreeSwitch.Core"
    VS_DOTNET_ASSEMBLY_NAME "FreeSwitch.Core"
)

###############################################################################
# Setup inter-target dependency
###############################################################################

# Ensure that the managed assembly builds before the native module,
# as the native code will load the managed assembly at runtime.
add_dependencies(mod_core FreeSwitch.Core)

###############################################################################
# Installation Rules (Optional)
###############################################################################

install(TARGETS mod_core DESTINATION ${CMAKE_INSTALL_PREFIX}/modules)
install(TARGETS FreeSwitch.Core DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/core)

###############################################################################
# Optional: Custom Commands for SWIG
###############################################################################
# If you need to run SWIG to generate interop code before building the managed target,
# you can add a custom target here.
#
# add_custom_target(runswig
#    COMMAND ${CMAKE_COMMAND} -E echo "Running SWIG to generate managed wrappers..."
#    COMMAND <your swig command here>
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
# )
#
# Then, ensure that FreeSwitch.Core depends on the runswig target:
# add_dependencies(FreeSwitch.Core runswig)
