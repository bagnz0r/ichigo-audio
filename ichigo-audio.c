// Ichigo Audio Library - A core audio library for Ichigo
// Copyright (C) 2014 bagnz0r (http://github.com/bagnz0r)

// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <stdbool.h>
#include <bass.h>

#ifndef OSX
#include <bass_aac.h>
#include <bass_alac.h>
#endif

#include <bass_fx.h>
#include <bassflac.h>

int current_device = 0;
int current_stream = -1;

bool end_of_stream = true;
bool paused = true;

void __stdcall sync_end(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	BASS_ChannelStop(current_stream);
	BASS_StreamFree(current_stream);

	current_stream = -1;

	end_of_stream = true;
	paused = true;
}

char * get_filename_ext(char * file_name) {
	char * lwr = tolower(file_name);
    char * dot = strrchr(lwr, '.');
    if (!dot || strcmp(dot, lwr))
    	return "";
    return dot + 1;
}

bool ig_initialize(int device, int freq)
{
	if (BASS_Init(device, freq, BASS_DEVICE_FREQ, 0, NULL))
	{
		current_device = device;
		return true;
	}

	return false;
}

void ig_create_stream(wchar_t * file_name)
{
	if (current_stream != -1)
	{
		BASS_ChannelStop(current_stream);
		BASS_StreamFree(current_stream);
		
		current_stream = -1;
	}

	char * ext = get_filename_ext((char *)file_name);

	#ifndef OSX
	if (!strcmp(ext, "m4a") || !strcmp(ext, "aac"))
	{
		current_stream = BASS_AAC_StreamCreateFile(0, file_name, 0, 0, BASS_SAMPLE_FLOAT | BASS_UNICODE);
		if (!current_stream)
		{
			current_stream = BASS_ALAC_StreamCreateFile(0, file_name, 0, 0, BASS_SAMPLE_FLOAT | BASS_UNICODE);
			if (!current_stream)
			{
				current_stream = BASS_MP4_StreamCreateFile(0, file_name, 0, 0, BASS_SAMPLE_FLOAT | BASS_UNICODE);
			}
		}
	}
	else
	#endif
		
	if (!strcmp(ext, "flac"))
	{
		current_stream = BASS_FLAC_StreamCreateFile(0, file_name, 0, 0, BASS_SAMPLE_FLOAT | BASS_UNICODE);
	}
	else
	{
		current_stream = BASS_StreamCreateFile(0, file_name, 0, 0, BASS_SAMPLE_FLOAT | BASS_UNICODE);
	}
}

void ig_play()
{
	if (current_stream == -1)
		return;

	BASS_ChannelPlay(current_stream, 0);
	BASS_ChannelSetSync(current_stream, BASS_SYNC_END, 0, &sync_end, 0);

	end_of_stream = false;
	paused = false;
}

void ig_pause()
{
	if (current_stream == -1)
		return;

	BASS_ChannelPause(current_stream);

	paused = true;
}

void ig_stop()
{
	if (current_stream == -1)
		return;

	BASS_ChannelStop(current_stream);
	BASS_StreamFree(current_stream);
	current_stream = -1;

	end_of_stream = true;
	paused = true;
}

double ig_get_pos()
{
	if (current_stream == -1)
		return -1;

	return BASS_ChannelBytes2Seconds(current_stream, BASS_ChannelGetPosition(current_stream, BASS_POS_BYTE));
}

double ig_get_len()
{
	if (current_stream == -1)
		return -1;

	return BASS_ChannelBytes2Seconds(current_stream, BASS_ChannelGetLength(current_stream, BASS_POS_BYTE));
}

void ig_set_pos(double position)
{
	if (current_stream == -1)
		return;

	BASS_ChannelSetPosition(current_stream, BASS_ChannelSeconds2Bytes(current_stream, position), BASS_POS_BYTE);
}

float ig_get_volume()
{
	return BASS_GetVolume();
}

void ig_set_volume(float volume)
{
	BASS_SetVolume(volume);
}

bool ig_is_stream_active()
{
	return !end_of_stream;
}

bool ig_is_paused()
{
	if (current_stream == -1)
		return false;

	return paused;
}
