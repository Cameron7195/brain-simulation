#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2
  make -f /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2
  make -f /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2
  make -f /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2
  make -f /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2/CMakeScripts/ReRunCMake.make
fi

