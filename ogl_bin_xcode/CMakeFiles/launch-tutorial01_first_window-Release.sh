#!/bin/sh
bindir=$(pwd)
cd /Users/Cameron/Projects/ogl-master/tutorial01_first_window/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial01_first_window 
	else
		"/Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial01_first_window"  
	fi
else
	"/Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial01_first_window"  
fi
