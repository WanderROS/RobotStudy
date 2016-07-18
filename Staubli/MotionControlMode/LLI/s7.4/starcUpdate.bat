@ECHO OFF
set l_ipAdress=%1
set l_login=%2
set l_password=%3

REM Set temp directory
IF NOT "%TEMP%"=="" set l_temp=%TEMP%\cs8_backup
IF "%TEMP%"=="" set l_temp=C:\TEMP\cs8_backup
IF NOT EXIST %l_temp%       MD %l_temp%
set l_temp=%l_temp%\%l_ipAdress%

IF "%l_ipAdress%"=="" GOTO ERROR										
IF "%l_login%"=="" GOTO ERROR										
IF "%l_password%"=="" GOTO ERROR										

REM Check that all files to update are present
set l_file=""
IF NOT EXIST _replaceStr.exe set l_file=_replaceStr.exe
IF NOT EXIST flash\sys\starc\starc.zfx set l_file=flash\sys\starc\starc.zfx
IF NOT %l_file% == "" GOTO INVALID

REM Build directories needed for backup
IF NOT EXIST %l_temp%       MD %l_temp%
IF NOT EXIST %l_temp%	GOTO ERROR_TEMP
IF NOT EXIST %l_temp%\sys MD %l_temp%\sys				
IF EXIST %l_temp%\options.cfx DEL %l_temp%\options.cfx

:TARGET
type _starcFtp.txt | _replaceStr #TEMP# %l_temp% | _replaceStr #LOGIN# %l_login% | _replaceStr #PASSWORD# %l_password% > %l_temp%\starcFtp.txt

:FTP
REM Launch update script if bootline.dat file has been built successfully
ECHO.															
ECHO  Use backup.bat command to first backup controller version.
ECHO  New files will be send to the target %l_ipAdress%.				
ECHO IMPORTANT - After installation, an adjustment of arm position will be required.
ECHO IMPORTANT - Make sure the arm is stopped at a known position.
ECHO  Press Ctrl+C to stop process.
PAUSE															
CALL ftp -i -v -n -s:%l_temp%\starcFtp.txt %l_ipAdress%
IF NOT "%l_robot%"==""	CALL ftp -i -v -n -s:%l_temp%\ftpOptions.txt %l_ipAdress%
ECHO.
ECHO --------------------------------------------------
ECHO  Please check that there was no network error during update before reboot.
ECHO --------------------------------------------------		
ECHO.															
GOTO END														
																
:ERROR															
ECHO.															
ECHO ERROR IN PARAMETER. Format is :	
ECHO starcUpdate ip_adress login ftpPassword
GOTO END

:ERROR_TEMP
ECHO.
ECHO ERROR Could not create %l_temp% for backup
PAUSE															
GOTO END
			
:INVALID
ECHO.
ECHO ERROR: Incomplete software reference
ECHO %l_file% is missing
ECHO Check the content of flash directory
GOTO END														
															
:END															
																
