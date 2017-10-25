Option explicit

Dim	wmi, os, value, wsh, sha, sfo, btn

Set sfo = CreateObject("Scripting.FileSystemObject")

do while WScript.Arguments.Count=0 and UCDbl(WScript.Version) >= 5.7
  'Check if Vista
  Set wmi = GetObject("winmgmts:" & "{impersonationLevel=impersonate}!\\.\root\cimv2")
  Set os = wmi.ExecQuery ("SELECT * FROM Win32_OperatingSystem")
  For Each value in os
    if UCDbl(left(value.Version,3))<6.0 then exit do 'Exit if not Vista
  Next

  'Run this script as admin.
  Set sha = CreateObject("Shell.Application")
  sha.ShellExecute "wscript.exe",""""+WScript.ScriptFullName+""" vista","","runas"
 
  WScript.Quit
loop

'Current directory may be moved to system32, maybe not.
Set wsh = CreateObject("WScript.Shell")

btn = wsh.Popup("Yes: Fast Transfer,  No: Stable Transfer", 30, " Low-speed CDC", 4)
Select Case btn
    case 6  wsh.RegWrite "HKLM\SYSTEM\CurrentControlSet\Services\lowcdc\UseBulk\On", 1, "REG_DWORD"
    case 7  On Error Resume Next
		wsh.RegDelete "HKLM\SYSTEM\CurrentControlSet\Services\lowcdc\UseBulk\"
End Select

' Universal CDbl -- try to handle decimal separator differences
Function UCDbl( Str )
  Dim Sep
  ' Get current decimal separator (2nd char in computed and stringized 1.5)
  Sep = Mid( CStr( 3/2 ), 2, 1 )          
  ' Replace all separators with the valid one and explicitly convert do double
  ' If needed, add another separator into the chain of Replace() calls  
  UCDbl = CDbl( Replace( Replace( Str, ".", Sep ), ",", Sep ) )
End Function
