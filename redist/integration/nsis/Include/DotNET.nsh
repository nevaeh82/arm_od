!include WordFunc.nsh
!include LogicLib.nsh
!insertmacro VersionCompare

!macro CheckDotNET
SetOutPath $TEMP
    File "C:\Soft_CD\dotNetFramework 2\dotnetfx.exe"

!define DOTNET_VERSION "2.0.50727"

DetailPrint "�������� ������� .NET Framework..."
CheckVersion:
StrCpy $8 ""

	Push $0
	Push $1
	 
	System::Call "mscoree::GetCORVersion(w .r0, i ${NSIS_MAX_STRLEN}, *i) i .r1"
	StrCmp $1 0 +2
		StrCpy $0 "none"
	 
	Pop $1
	Exch $0
	Pop $0
	${If} $0 == "none"
		DetailPrint ".NET Framework �����������"
	  ${If} $8 == ""
			Goto RunInstall
		${Else}
			Goto InvalidDotNetAfterInstall
		${EndIf}
	${EndIf}

    DetailPrint $0
	 
	StrCpy $0 $0 "" 1 # skip "v"
	${VersionCompare} $0 ${DOTNET_VERSION} $1
	${If} $1 == 2
		DetailPrint ".NET Framework ������, ������: $0,"
        DetailPrint " ���������� ���������� ������: ${DOTNET_VERSION}"
	  ${If} $8 == ""
			Goto RunInstall
		${Else}
			Goto InvalidDotNetAfterInstall
		${EndIf}
	${EndIf}
	Goto ValidDotNET
		
RunInstall:
    push $2
    push $3
    FindFirst $2 $3 "$TEMP\dotnetfx.exe"
    DetailPrint $2
    DetailPrint $3
    strcmp $3 "" 0 +4    
    DetailPrint "�� ������ ���� ��������� .NET Framework"
    MessageBox MB_OK|MB_ICONSTOP|MB_DEFBUTTON1 "�� ������ ���� ��������� .NET Framework" /SD IDOK
    abort
    
	NsisDotNetInstaller::LaunchInstaller "$TEMP\dotnetfx.exe"
	DetailPrint "���������� ��������� Framework ${DOTNET_VERSION}"
    Delete "$TEMP\dotnetfx.exe"
    goto CheckVersion
    
InvalidDotNetAfterInstall:
    DetailPrint "������ ��� ��������� .NET Framework"
    MessageBox MB_OK|MB_ICONSTOP|MB_DEFBUTTON1 "������ ��� ��������� .NET Framework" /SD IDOK
    abort
	
ValidDotNET:
	DetailPrint ".NET Framework � ���������� ���������"
!macroend