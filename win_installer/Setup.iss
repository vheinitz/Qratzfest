;InnoSetup installer-builder script for Qratzfest.
;V.Heinitz, 2014-08-16


[Setup]
AppName=Qratzfest
AppVerName=Qratzfest 0.1
OutputBaseFilename=QratzfestSetup_0.1
AppPublisher=Valentin Heinitz
AppPublisherURL=http://www.heinitz-it.de
AppSupportURL=http://www.heinitz-it.de
AppUpdatesURL=http://www.heinitz-it.de
AppCopyright=Copyright © 2014 Valentin Heinitz
DefaultDirName={pf}\\heinitz-it\\Qratzfest
DefaultGroupName=heinitz-it\Qratzfest
DisableDirPage=no
DisableProgramGroupPage=yes
AllowNoIcons=yes
OutputDir=..\..\Releases
BackColor=clGreen
;SetupIconFile=..\Qratzfest\icon.ico
Compression=lzma
WizardImageFile=.\res\Setup.bmp
WizardSmallImageFile=.\res\Setup_s.bmp
SolidCompression=yes


[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]

Source: "c:\Qt\4.8.4\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\Qt\4.8.4\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\Qt\4.8.4\bin\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\release\Qratzfest.exe"; DestDir: "{app}"; Flags: ignoreversion


;Source: "..\data\Readme.txt"; DestDir: "{userdocs}\\heinitz-it\\Qratzfest\\"; Flags: ignoreversion


[Icons]
Name: "{group}\Qratzfest"; Filename: "{app}\Qratzfest.exe"
Name: "{group}\{cm:UninstallProgram,Qratzfest}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Qratzfest"; Filename: "{app}\Qratzfest.exe"; Tasks: desktopicon
;Name: "{group}\Readme"; Filename: "{userdocs}\\heinitz-it\\Qratzfest\\Readme.txt"

