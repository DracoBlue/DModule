#!/bin/bash
DIR=`pwd`
echo ""
echo "Welcome to DracoBlue's Project Compiler"
echo ""
echo "      http://dracoblue.net"
echo ""

echo "Directory: " $DIR
if [ -z $1 ]
then
	echo "No project given!"
	exit 1
fi
cd $DIR
if [ ! -d "$1/build" ]
then
	mkdir "$1/build"
fi

if [ ! -d "$1/build/gamemodes" ]
then
	mkdir "$1/build/gamemodes"
fi

if [ ! -d "$1/build/scriptfiles" ]
then
	mkdir "$1/build/scriptfiles"
fi

if [ ! -d "$1/build/plugins" ]
then
	mkdir "$1/build/plugins"
fi

function clean {
if [ -f "$1/build.error.log" ]; then
	rm "$1/build.error.log"
fi
if [ -f "$1/compile.error.log" ]; then
	rm "$1/compile.error.log"
fi
if [ -f "$1/compile.info.log" ]; then
	rm "$1/compile.info.log"
fi
if [ -f "$1/debug_output.log" ]; then
	rm "$1/debug_output.log"
fi
}

clean $1

rm "$1/build/gamemodes/$1.pwn"
rm "$1/build/gamemodes/$1.amx"

cd ..
cd core/lua
wine lua make_project.lua $1 "wine"
cd $DIR
wine ../core/pawn/pawncc.exe "$1\\build\\gamemodes\\$1.pwn" -o"$1\\build\\gamemodes\\$1.amx" -r"$1\\compile.info.log"  -e"$1\\compile.error.log"  -O0 -";" -"("
cd ../core/lua
wine lua parse_errors.lua $1 "wine"
wine lua after_compile.lua $1 "wine"

cd $DIR
clean $1
echo ""
echo "Successfully built $1! Run the server at $1/build/samp-server.exe now!"

