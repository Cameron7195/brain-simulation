#!/bin/sh
bindir=$(pwd)
cd /Users/Cameron/Projects/ogl-master/tutorial18_billboards_and_particles/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial18_particles 
	else
		"/Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial18_particles"  
	fi
else
	"/Users/Cameron/Projects/ogl_bin_xcode/Release/tutorial18_particles"  
fi
