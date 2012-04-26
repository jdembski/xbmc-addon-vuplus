@ECHO OFF
CLS
COLOR 1B
TITLE XBMC Addon for VUPlus
SET comp=vs2010
SET target=dx
SET buildmode=ask
SET promptlevel=prompt

SET exitcode=0
FOR %%b in (%1, %2, %3, %4, %5) DO (
	IF %%b==vs2010 SET comp=vs2010
	IF %%b==dx SET target=dx
	IF %%b==gl SET target=gl
	IF %%b==clean SET buildmode=clean
)

SET buildconfig=Release (DirectX)
IF %target%==gl SET buildconfig=Release (OpenGL)

IF %comp%==vs2010 (
  IF "%VS100COMNTOOLS%"=="" (
		set NET="%ProgramFiles%\Microsoft Visual Studio 10.0\Common7\IDE\VCExpress.exe"
	) ELSE IF EXIST "%VS100COMNTOOLS%\..\IDE\VCExpress.exe" (
		set NET="%VS100COMNTOOLS%\..\IDE\VCExpress.exe"
	) ELSE IF EXIST "%VS100COMNTOOLS%\..\IDE\devenv.exe" (
		set NET="%VS100COMNTOOLS%\..\IDE\devenv.exe"
	)
)

  IF NOT EXIST %NET% (
     set DIETEXT=Visual Studio .NET 2010 Express was not found.
	 goto DIE
  )

set OPTS_EXE=xbmc-addon-vuplus.sln /build %buildconfig%
set CLEAN_EXE=xbmc-addon-vuplus.sln /clean %buildconfig%


ECHO Cleaning Solution...
%NET% %CLEAN_EXE%
ECHO Compiling Addon for XBMC...
%NET% %OPTS_EXE%

IF EXIST addons\pvr.vuplus\changelog.txt del addons\pvr.vuplus\changelog.txt > NUL
IF EXIST addons\pvr.vuplus\addon.xml del addons\pvr.vuplus\addon.xml > NUL

copy ChangeLog addons\pvr.vuplus\changelog.txt


REM -------Version aus ChangeLog in addons/pvr.vuplus/addon.xml eintragen--------

setlocal enabledelayedexpansion 

for /F "delims=" %%i in (ChangeLog) do if not defined zeile set "zeile=%%i"
set zeile=!zeile:~0,-1!
set "Von=  version" 
set "Nach=  version="X"" 
set Nach=!Nach:X=%zeile%!

if exist addon.xml del addon.xml

for /f "usebackq delims=" %%i in ("addons\pvr.vuplus\addon.xml.in") do (
set "Line=%%i"
set Line=!Line:%Von%=%Nach%!
set Line1=!Line!
set line1=!line1:~0,9!
set line1=!line1:~2,7!

 IF !Line1!==version set Line=!Line:~0,-12!

Echo !Line!>>addons\pvr.vuplus\addon.xml 
)
setlocal disabledelayedexpansion 

REM ------------------------------------------------------------------------------
 

set ZIP="%ProgramFiles%\7-Zip\7z.exe"
IF NOT EXIST %ZIP% (
   set DIETEXT=7zip was not found.
     goto DIE
)

IF EXIST pvr.vuplus.zip del pvr.vuplus.zip > NUL
 
 cd addons
%ZIP% a pvr.vuplus.zip  pvr.vuplus -xr!*.in -xr!*.am -xr!*.exp -xr!*.ilk >NUL
move pvr.vuplus.zip ..\ > NUL

cd ..
goto END

:DIE
  ECHO ------------------------------------------------------------
  ECHO !-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-
  ECHO    ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR
  ECHO !-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-
  set DIETEXT=ERROR: %DIETEXT%
  echo %DIETEXT%
  SET exitcode=1
  ECHO ------------------------------------------------------------

:END
IF %promptlevel% NEQ noprompt (
ECHO --------------------------------------------------------------- 
ECHO The file "pvr.vuplus.zip" was created in the current directory.
ECHO --------------------------------------------------------------- 
ECHO Press any key to exit...
pause > NUL
)
EXIT /B %exitcode%

