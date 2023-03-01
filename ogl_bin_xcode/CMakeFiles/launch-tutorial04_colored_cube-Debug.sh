#!/bin/sh
bindir=$(pwd)
cd /Users/Cameron/Projects/ogl-master/tutorial04_colored_cube/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/Cameron/Projects/ogl_bin_xcode/Debug/tutorial04_colored_cube 
	else
		"/Users/Cameron/Projects/ogl_bin_xcode/Debug/tutorial04_colored_cube"  
	fi
else
	"/Users/Cameron/Projects/ogl_bin_xcode/Debug/tutorial04_colored_cube"  
fi
