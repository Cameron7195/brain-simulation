#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2
  /Applications/CMake.app/Contents/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2
  /Applications/CMake.app/Contents/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2
  /Applications/CMake.app/Contents/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2
  /Applications/CMake.app/Contents/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi

