#!/bin/bash
mkdir output

if [ $# -ne 1 ]; then
    echo "Missing platform argument"
    echo "Usage: build.sh [win32|osx|linux]"
    exit 1
fi
if [ $1 == "osx" ]; then
	ext="dylib"
	libs=""
	headers=""

	cp -v ichigo_audio.c ichigo_audio.build.c
	echo -e "#define OSX 1\n\n$(cat ichigo_audio.build.c)" > ichigo_audio.build.c

	cp -v dependencies/osx/bass/libbass.dylib output/libbass.dylib
	cp -v dependencies/osx/bass_fx/libbass_fx.dylib output/libbass_fx.dylib
	cp -v dependencies/osx/bassflac/libbassflac.dylib output/libbassflac.dylib
fi
if [ $1 == "win32" ]; then
	ext="dll"
	libs="-Ldependencies/win32/bass_aac -Ldependencies/win32/bass_alac -lbass_aac -lbass_alac"
	headers="-Idependencies/win32/bass_aac -Idependencies/win32/bass_alac"

	cp -v dependencies/win32/bass/bass.dll output/bass.dll
	cp -v dependencies/win32/bass_fx/bass_fx.dll output/bass_fx.dll
	cp -v dependencies/win32/bassflac/bassflac.dll output/bassflac.dll
	cp -v dependencies/win32/bass_aac/bass_aac.dll output/bass_aac.dll
	cp -v dependencies/win32/bass_alac/bass_alac.dll output/bass_alac.dll
fi
if [ $1 == "linux" ]; then
	ext="so"
	libs="-Ldependencies/win32/bass_aac -Ldependencies/win32/bass_alac -lbass_aac -lbass_alac"
	headers="-Idependencies/win32/bass_aac -Idependencies/win32/bass_alac"

	cp -v dependencies/linux/bass/libbass.so output/libbass.so
	cp -v dependencies/linux/bass_fx/libbass_fx.so output/libbass_fx.so
	cp -v dependencies/linux/bassflac/libbassflac.so output/libbassflac.so
	cp -v dependencies/linux/bass_aac/libbass_aac.so output/libbass_aac.so
	cp -v dependencies/linux/bass_alac/libbass_alac.so output/libbass_alac.so
fi

gcc -c ichigo_audio.build.c -Idependencies/$1/bass -Idependencies/$1/bassflac -Idependencies/$1/bass_fx $headers -m32 -o ichigo-audio.o
gcc -shared -Wl -m32 -Ldependencies/$1/bass -Ldependencies/$1/bassflac -Ldependencies/$1/bass_fx $libs -lbass -lbassflac -lbass_fx -o ichigo-audio.$ext ichigo-audio.o
rm ichigo_audio.build.c
rm ichigo-audio.o

cp -v ichigo-audio.$ext output/ichigo-audio.$ext
