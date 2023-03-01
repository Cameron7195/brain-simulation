#!/bin/sh
bindir=$(pwd)
cd /Users/Cameron/Projects/ogl-master/tutorial17_rotations/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial17_rotations 
	else
		"/Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial17_rotations"  
	fi
else
	"/Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial17_rotations"  
fi
