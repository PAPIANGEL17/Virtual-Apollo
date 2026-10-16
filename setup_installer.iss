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
; Rutas adaptadas a la estructura de salida de Ninja y CMake
Source: "build/VirtualApolloCore.exe"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "build/VirtualApolloCore_artefacts/Release/Standalone/VirtualApolloCore.exe"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

[Registry]
Root: HKLM; Subkey: "SOFTWARE\ASIO\Virtual Apollo ASIO"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\ASIO\Virtual Apollo ASIO"; ValueType: string; ValueName: "CLSID"; ValueData: "{{B1234567-89AB-CDEF-0123-456789ABCDEF}}"
Root: HKLM; Subkey: "SOFTWARE\ASIO\Virtual Apollo ASIO"; ValueType: string; ValueName: "Description"; ValueData: "Virtual Apollo Zero-Latency DSP Core"

[Icons]
Name: "{group}\Virtual Apollo Control Panel"; Filename: "{app}\VirtualApolloCore.exe"
Name: "{commondesktop}\Virtual Apollo Control Panel"; Filename: "{app}\VirtualApolloCore.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Crear acceso directo en el escritorio"; GroupDescription: "Adicionales:"
