#!/bin/sh
bindir=$(pwd)
cd /Users/karre/Graphics/OpenGL-tutorial_v0014_21/tutorial11_2d_fonts/
export DYLD_LIBRARY_PATH=:$DYLD_LIBRARY_PATH

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		 -batch -command=$bindir/gdbscript  /Users/karre/Graphics/OpenGL_bin_XCode/Debug/tutorial11_2d_fonts 
	else
		"/Users/karre/Graphics/OpenGL_bin_XCode/Debug/tutorial11_2d_fonts"  
	fi
else
	"/Users/karre/Graphics/OpenGL_bin_XCode/Debug/tutorial11_2d_fonts"  
fi
