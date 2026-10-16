[Setup]
AppName=Virtual Apollo ASIO Driver
AppVersion=2.0.0
DefaultDirName={commoncf}\ASIO\VirtualApollo
DefaultGroupName=Virtual Apollo
OutputDir=.
OutputBaseFilename=Virtual_Apollo_Setup_v2.0
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin

[Files]
Source: "build/VirtualApolloCore_artefacts/Release/Standalone/VirtualApolloCore.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "build/VirtualApolloCore_artefacts/Release/Standalone/VirtualApolloASIO.dll"; DestDir: "{sys}"; Flags: ignoreversion 32bit64bit regserver

[Registry]
; Registro oficial en Windows para que aparezca en el panel ASIO de cualquier DAW
Root: HKLM; Subkey: "SOFTWARE\ASIO\Virtual Apollo ASIO"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\ASIO\Virtual Apollo ASIO"; ValueType: string; ValueName: "CLSID"; ValueData: "{B1234567-89AB-CDEF-0123-456789ABCDEF}"
Root: HKLM; Subkey: "SOFTWARE\ASIO\Virtual Apollo ASIO"; ValueType: string; ValueName: "Description"; ValueData: "Virtual Apollo Zero-Latency DSP Core"

[Icons]
Name: "{group}\Virtual Apollo Control Panel"; Filename: "{app}\VirtualApolloCore.exe"
Name: "{commondesktop}\Virtual Apollo Control Panel"; Filename: "{app}\VirtualApolloCore.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Crear acceso directo en el escritorio"; GroupDescription: "Adicionales:"