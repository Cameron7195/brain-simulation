#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2/docs
  /usr/local/bin/doxygen
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2/docs
  /usr/local/bin/doxygen
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2/docs
  /usr/local/bin/doxygen
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/Cameron/Projects/ogl_bin_xcode/external/glfw-3.1.2/docs
  /usr/local/bin/doxygen
fi

