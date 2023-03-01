#!/bin/sh
bindir=$(pwd)
cd /Users/Cameron/Projects/ogl-master/misc05_picking/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/Cameron/Projects/ogl_bin_xcode/MinSizeRel/misc05_picking_custom 
	else
		"/Users/Cameron/Projects/ogl_bin_xcode/MinSizeRel/misc05_picking_custom"  
	fi
else
	"/Users/Cameron/Projects/ogl_bin_xcode/MinSizeRel/misc05_picking_custom"  
fi
