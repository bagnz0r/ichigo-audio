#!/bin/bash
mkdir output

if [ $# -ne 1 ]; then
    echo "Missing platform argument"
    echo "Usage: build.sh [osx|linux]"
    exit 1
fi
if [ $1 == "osx" ]; then
	ext="dylib"
	libs=""
	headers=""
	arch=""
	std=""

	cp -v dependencies/osx/bass/libbass.dylib output/libbass.dylib
	cp -v dependencies/osx/bass_fx/libbass_fx.dylib output/libbass_fx.dylib
	cp -v dependencies/osx/bassflac/libbassflac.dylib output/libbassflac.dylib
	cp -v dependencies/osx/basswv/libbasswv.dylib output/libbasswv.dylib
	cp -v dependencies/osx/bass_ape/libbass_ape.dylib output/libbass_ape.dylib
	cp -v dependencies/osx/bass_mpc/libbass_mpc.dylib output/libbass_mpc.dylib
	cp -v dependencies/osx/tags/libtags.dylib output/libtags.dylib

	cp -v ichigo-audio.c ichigo-audio.build.c
fi
if [ $1 == "linux" ]; then
	ext="so"
	libs="-Ldependencies/linux/bass_aac -Ldependencies/linux/bass_alac -lbass_aac -lbass_alac"
	headers="-Idependencies/linux/bass_aac -Idependencies/linux/bass_alac"
	arch=""
	std="-std=gnu11"

	cp -v dependencies/linux/bass/x64/libbass.so output/libbass.so
	cp -v dependencies/linux/bass_fx/x64/libbass_fx.so output/libbass_fx.so
	cp -v dependencies/linux/bassflac/x64/libbassflac.so output/libbassflac.so
	cp -v dependencies/linux/bass_aac/x64/libbass_aac.so output/libbass_aac.so
	cp -v dependencies/linux/bass_alac/x64/libbass_alac.so output/libbass_alac.so
	cp -v dependencies/linux/basswv/x64/libbasswv.so output/libbasswv.so
	cp -v dependencies/linux/bass_ape/x64/libbass_ape.so output/libbass_ape.so
	cp -v dependencies/linux/bass_mpc/x64/libbass_mpc.so output/libbass_mpc.so
	cp -v dependencies/linux/tags/x64/libtags.so output/libtags.so

	cp -v ichigo-audio.c ichigo-audio.build.c
fi

gcc $std -w -fPIC -x c -c ichigo-audio.build.c -Idependencies/$1/bass -Idependencies/$1/bassflac -Idependencies/$1/bass_fx -Idependencies/$1/tags/c $headers -o ichigo-audio.o
gcc $std -w -shared -Ldependencies/$1/bass -Ldependencies/$1/bassflac -Ldependencies/$1/bass_fx -Ldependencies/$1/basswv -Ldependencies/$1/bass_ape -Ldependencies/$1/bass_mpc -Ldependencies/$1/tags $libs -lbass -lbassflac -lbass_fx -lbasswv -lbass_ape -lbass_mpc -ltags -o ichigo-audio.$ext ichigo-audio.o

rm ichigo-audio.build.c
rm ichigo-audio.o

cp -v ichigo-audio.$ext output/ichigo-audio.$ext
