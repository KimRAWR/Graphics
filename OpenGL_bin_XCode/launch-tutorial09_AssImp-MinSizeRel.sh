#!/bin/sh
bindir=$(pwd)
cd /Users/karre/Graphics/OpenGL-tutorial_v0014_21/tutorial09_vbo_indexing/
export DYLD_LIBRARY_PATH=:$DYLD_LIBRARY_PATH

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		 -batch -command=$bindir/gdbscript  /Users/karre/Graphics/OpenGL_bin_XCode/MinSizeRel/tutorial09_AssImp 
	else
		"/Users/karre/Graphics/OpenGL_bin_XCode/MinSizeRel/tutorial09_AssImp"  
	fi
else
	"/Users/karre/Graphics/OpenGL_bin_XCode/MinSizeRel/tutorial09_AssImp"  
fi
