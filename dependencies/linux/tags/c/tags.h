//////////////////////////////////////////////////////////////////////////
//
// tags.h - TAGS: Yet Another Tag Reading Library for BASS 2.3+
//
// Author: Wraith, 2k5-2k6
// Public domain. No warranty.
//
// (public)
// 
// Abstract:    reads tags from given BASS handle, formats them according
//              to given format string and returns the resulting string.
//           
// read tags-readme.txt for details
//

#ifndef _YATRL_H_W2348_H4232
#define _YATRL_H_W2348_H4232

// c guards (hmm... just in case)
#ifdef __cplusplus
extern "C" {
#endif

// Current version. Just increments each release.
#define TAGS_VERSION 17

// returns description of the last error.
__attribute__((visibility("default"))) const char* TAGS_GetLastErrorDesc();

// main purpose of this library
__attribute__((visibility("default"))) const char*  TAGS_Read( DWORD dwHandle, const char* fmt );

// retrieves the current version
__attribute__((visibility("default"))) DWORD TAGS_GetVersion();

#ifdef __cplusplus
}
#endif


#endif
