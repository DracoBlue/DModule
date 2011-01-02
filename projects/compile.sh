#!/bin/bash
DIR=`pwd`
echo ""
echo "Welcome to DracoBlue's Project Compiler"
echo ""
echo "      http://dracoblue.net"
echo ""

echo "Directory: " $DIR
echo "Arguments: " $1 $2 $3
cd $DIR
mkdir "$1/build"
mkdir "$1/build/gamemodes"
mkdir "$1/build/plugins"
rm "$1/build.error.log"
rm "$1/compile.error.log"
rm "$1/compile.info.log"
rm "$1/debug_output.log"
rm "$1/build/gamemodes/$1.pwn"
rm "$1/build/gamemodes/$1.amx"

cd ..
icd core/lua
lua make_project.lua $1 "linux"
cd $DIR
pwd
pawncc "$1/build/gamemodes/$1.pwn" -o"$1/build/gamemodes/$1.amx" -r"$1/compile.info.log"  -e"$1/compile.error.log"  -O0 -";" -"("
cd ../core/lua
lua parse_errors.lua $1 "linux"
lua after_compile.lua $1 "linux"

cd $DIR
echo ""
echo "Successfully built $1! Run the server at $1/build/samp-server.exe now!"

