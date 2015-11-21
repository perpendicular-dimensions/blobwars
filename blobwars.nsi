# Blob Wars : Metal Blob Solid script

Name "Blob Wars : Metal Blob Solid"
OutFile "blobwars-2.00-1.installer.exe"
InstallDir "$PROGRAMFILES\Parallel Realities\BlobWars"
Page Directory
Page InstFiles

Section "Main"
  SetOutPath $INSTDIR
  File blobwars.exe
  File blobwars.pak
  File icons/blobwars.ico
  File *.dll
  File /r locale
  File /r doc
SectionEnd

Section "Shortcuts"
	CreateDirectory "$SMPROGRAMS\Parallel Realities\BlobWars"
	CreateShortCut "$SMPROGRAMS\Parallel Realities\BlobWars\BlobWars.lnk" "$INSTDIR\blobwars.exe" "" $INSTDIR\blobwars.ico 0 "" ""
	CreateShortCut "$SMPROGRAMS\Parallel Realities\BlobWars\Uninstall.lnk" "$INSTDIR\uninstaller.exe"
	CreateShortCut "$DESKTOP\BlobWars.lnk" "$INSTDIR\blobwars.exe" "" "$INSTDIR\blobwars.ico" 0 "" ""
SectionEnd

Section "CreateUninstaller"
	WriteUninstaller "$INSTDIR\uninstaller.exe"
SectionEnd

Section "Uninstall"
  Delete "$SMPROGRAMS\Parallel Realities\BlobWars\*.*"
  Delete "$SMPROGRAMS\Parallel Realities\BlobWars"
  Delete "$DESKTOP\BlobWars.lnk"
  RMDir /r "$INSTDIR"
SectionEnd
