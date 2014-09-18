// Ichigo Audio Library - A core audio library for Ichigo
// Copyright (C) 2014 bagnz0r (http://github.com/bagnz0r)

// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include <bass.h>
#include <bass_fx.h>
#include <tags.h>

#ifdef _WIN32
#define PF __declspec(dllexport)
#else
#define PF __attribute__((visibility("default")))
#endif

// Effects enabled?
bool fx = false;

// Currently selected audio device.
int current_device = 0;

// Curent stream handle.
int current_stream = -1;

// Has the stream ended?
bool end_of_stream = true;

// Is the stream paused?
bool paused = true;

// Audio volume.
float audio_volume = 1;

#ifdef _WIN32
void __stdcall sync_end(HSYNC handle, DWORD channel, DWORD data, void *user)
#else
void sync_end(HSYNC handle, DWORD channel, DWORD data, void *user)
#endif
{
	BASS_ChannelStop(current_stream);
	BASS_StreamFree(current_stream);

	current_stream = -1;

	end_of_stream = true;
	paused = true;
}

//
// Initialize the Ichigo Audio library
//
PF bool ig_initialize(int device, int freq)
{
	if (!BASS_Init(device, freq, BASS_DEVICE_FREQ, 0, NULL))
	{
		return false;
	}

	current_device = device;

	// load all the plugins here
#ifdef _WIN32
	BASS_PluginLoad("bass_aac.dll", 0);
	BASS_PluginLoad("bass_alac.dll", 0);
	BASS_PluginLoad("bass_fx.dll", 0);
	BASS_PluginLoad("bassflac.dll", 0);
	BASS_PluginLoad("basswv.dll", 0);
	BASS_PluginLoad("bass_mpc.dll", 0);
	BASS_PluginLoad("bass_ape.dll", 0);
#elif defined(__linux__)
	BASS_PluginLoad("libbass_fx.so", 0);
	BASS_PluginLoad("libbassflac.so", 0);
	BASS_PluginLoad("libbasswv.so", 0);
	BASS_PluginLoad("libbass_mpc.so", 0);
	BASS_PluginLoad("libbass_ape.so", 0);
#else
	BASS_PluginLoad("libbass_fx.dylib", 0);
	BASS_PluginLoad("libbassflac.dylib", 0);
	BASS_PluginLoad("libbasswv.dylib", 0);
	BASS_PluginLoad("libbass_mpc.dylib", 0);
	BASS_PluginLoad("libbass_ape.dylib", 0);
#endif

	return true;
}

//
// Create audio stream from file
//
#ifdef _WIN32
PF int ig_create_stream(wchar_t * file_name)
#else
PF int ig_create_stream(char * file_name)
#endif
{
	if (current_stream != -1)
	{
		BASS_ChannelStop(current_stream);
		BASS_StreamFree(current_stream);
		
		current_stream = -1;
	}

#ifdef _WIN32
	current_stream = BASS_StreamCreateFile(false, file_name, 0, 0, BASS_SAMPLE_FLOAT | BASS_UNICODE);
#else
    current_stream = BASS_StreamCreateFile(false, file_name, 0, 0, BASS_SAMPLE_FLOAT);
#endif
    
    ig_set_volume(audio_volume);
    
	return BASS_ErrorGetCode();
}

//
// Create audio stream from URL
//
PF void ig_create_stream_from_url(char * url)
{
	if (current_stream != -1)
	{
		BASS_ChannelStop(current_stream);
		BASS_StreamFree(current_stream);

		current_stream = -1;
	}

	current_stream = BASS_StreamCreateURL(url, 0, BASS_SAMPLE_FLOAT | BASS_STREAM_RESTRATE, NULL, NULL);
    
    ig_set_volume(audio_volume);
}

//
// Play the current stream
//
PF void ig_play()
{
	if (current_stream == -1)
		return;

	BASS_ChannelPlay(current_stream, 0);
	BASS_ChannelSetSync(current_stream, BASS_SYNC_END, 0, &sync_end, 0);

	end_of_stream = false;
	paused = false;
}

//
// Pause the current stream
//
PF void ig_pause()
{
	if (current_stream == -1)
		return;

	BASS_ChannelPause(current_stream);

	paused = true;
}

//
// Stop the current stream
//
PF void ig_stop()
{
	if (current_stream == -1)
		return;

	BASS_ChannelStop(current_stream);
	BASS_StreamFree(current_stream);
	current_stream = -1;

	end_of_stream = true;
	paused = true;
}

//
// Get current track position
//
PF double ig_get_pos()
{
	if (current_stream == -1)
		return -1;

	return BASS_ChannelBytes2Seconds(current_stream, BASS_ChannelGetPosition(current_stream, BASS_POS_BYTE));
}

//
// Get track length
//
PF double ig_get_len()
{
	if (current_stream == -1)
		return -1;

	return BASS_ChannelBytes2Seconds(current_stream, BASS_ChannelGetLength(current_stream, BASS_POS_BYTE));
}

//
// Set track position
//
PF void ig_set_pos(double position)
{
	if (current_stream == -1)
		return;

	BASS_ChannelSetPosition(current_stream, BASS_ChannelSeconds2Bytes(current_stream, position), BASS_POS_BYTE);
}

//
// Get current volume
//
PF float ig_get_volume()
{
	if (current_stream == -1)
		return 0;

	float volume = 0;
	if (!BASS_ChannelGetAttribute(current_stream, BASS_ATTRIB_VOL, &volume))
		return 0;
	
	return volume;
}

//
// Set volume
//
PF void ig_set_volume(float volume)
{
	if (current_stream == -1)
		return;
    
    audio_volume = volume;

	BASS_ChannelSetAttribute(current_stream, BASS_ATTRIB_VOL, volume);
}

//
// Determines whether the stream is active/has ended or not
//
PF bool ig_is_stream_active()
{
	return !end_of_stream;
}

//
// Determines whether the stream has paused or not
//
PF bool ig_is_paused()
{
	if (current_stream == -1)
		return false;

	return paused;
}

//
// Reads tags on a current stream using the expression provided in tag_format
// Encapsulate your expression in %UTF8(expression_here) in order to get UTF8 encoded output
//
// For more information on expression format, please see tags-readme.txt in dependencies/{OS}/tags
//
PF char * ig_read_tag_from_current_stream(char * tag_format)
{
	return TAGS_Read(current_stream, tag_format);
}

//
// Creates a dummy stream and reads tags using the expression provided in tag_format
// Encapsulate your expression in %UTF8(expression_here) in order to get UTF8 encoded output
//
// For more information on expression format, please see tags-readme.txt in dependencies/{OS}/tags
//
#ifdef _WIN32
PF char * ig_read_tag_from_file(wchar_t * file_name, char * tag_format)
#else
PF char * ig_read_tag_from_file(char * file_name, char * tag_format)
#endif
{
#ifdef _WIN32
	int stream = BASS_StreamCreateFile(false, file_name, 0, 0, BASS_SAMPLE_FLOAT | BASS_UNICODE);
#else
	int stream = BASS_StreamCreateFile(false, file_name, 0, 0, BASS_SAMPLE_FLOAT);
#endif
	char * tag = TAGS_Read(stream, tag_format);
	BASS_StreamFree(stream);

	return tag;
}

//
// Enables the equalizer
//
PF void ig_enable_equalizer()
{
	fx = true;
}

//
// Disables the equalizer
//
PF void ig_disable_equalizer()
{
	fx = false;
}

//
// Sets gain value on the specified equalizer band
//
// band: 0..n
// freq: 1...n
// gain 0..n
//
int equalizer[18];
PF void ig_set_equalizer(int band, float freq, float gain)
{
	BASS_BFX_PEAKEQ param;
	param.lBand = band;
	if (!BASS_FXGetParameters(equalizer[band], &param)) {
		param.fBandwidth = 2.5;
		param.fCenter = freq;
		param.fQ = 0;

		equalizer[band] = BASS_ChannelSetFX(current_stream, BASS_FX_BFX_PEAKEQ, 0);
	}

	param.fGain = gain;
	BASS_FXSetParameters(equalizer[band], &param);
}