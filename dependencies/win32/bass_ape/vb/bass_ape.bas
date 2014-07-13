Attribute VB_Name = "bass_ape"
Option Explicit

' BASS_CHANNELINFO type
Global Const BASS_CTYPE_STREAM_APE = &H10700

Declare Function BASS_APE_StreamCreateFile64 Lib "bass_ape.dll" Alias "BASS_APE_StreamCreateFile" (ByVal mem As Long, ByVal file As Any, ByVal offset As Long, ByVal offsethigh As Long, ByVal length As Long, ByVal lengthhigh As Long, ByVal flags As Long) As Long
Declare Function BASS_APE_StreamCreateFileUser Lib "bass_ape.dll" (ByVal system As Long, ByVal flags As Long, ByVal procs As Long, ByVal user As Long) As Long

' 32-bit wrappers for 64-bit BASS functions
Function BASS_APE_StreamCreateFile(ByVal mem As Long, ByVal file As Long, ByVal offset As Long, ByVal length As Long, ByVal flags As Long) As Long
BASS_APE_StreamCreateFile = BASS_APE_StreamCreateFile64(mem, file, offset, 0, length, 0, flags Or BASS_UNICODE)
End Function
