@ECHO OFF
set l_mode=%1
set l_ipAdress=%2
set l_login=%3
set l_password=%4
set l_target=%5
set l_keep=false

REM Management choice LLI / SRC
if "%l_mode%"=="LLI" GOTO MINIJUMP
if "%l_mode%"=="SRC" GOTO MINIJUMP
GOTO ERROR

REM Management of optional target parameter
:MINIJUMP
if "%l_target%"=="teknor" GOTO ERROR_TEKNOR
if "%l_target%"=="men" GOTO ORDER
if "%l_target%"=="men_hp" GOTO ORDER
if "%l_target%"=="men007" GOTO ORDER
if "%l_target%"=="men07n" GOTO ORDER
REM if target = keep, bootline.dat will be kept unchanged
if "%l_target%"=="keep" set l_keep=true
if "%l_target%"=="keep" set l_target=
if NOT "%l_target%"=="" GOTO ERROR

REM Set temp directory
:ORDER
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
IF NOT EXIST _bootline.bat set l_file=_bootline.bat
IF NOT EXIST _ftpBootline.txt set l_file=_ftpBootline.txt
IF NOT EXIST _ftpCS8C_LLI.txt set l_file=_ftpCS8C_LLI.txt
IF NOT EXIST _ftpCS8C_SRC.txt set l_file=_ftpCS8C_SRC.txt
IF NOT EXIST flash\sys\men007 set l_file=flash\sys\men007
IF NOT EXIST flash\sys\men07n set l_file=flash\sys\men07n
IF NOT EXIST flash\sys\men_hp set l_file=flash\sys\men_hp
IF NOT EXIST flash\sys\men    set l_file=flash\sys\men
IF NOT EXIST flash\sys\src.out set l_file=flash\sys\src.out
IF NOT EXIST flash\sys\lli_test.out set l_file=flash\sys\lli_test.out
IF NOT EXIST flash\sys\men007.sys set l_file=flash\sys\men007.sys
IF NOT EXIST flash\sys\men07n.sys set l_file=flash\sys\men07n.sys
IF NOT EXIST flash\sys\men_hp.sys set l_file=flash\sys\men_hp.sys
IF NOT EXIST flash\sys\men.sys set l_file=flash\sys\men.sys
IF NOT EXIST flash\sys\teknor.sys set l_file=flash\sys\teknor.sys
IF NOT EXIST flash\sys\app\1_ep.o set l_file=flash\sys\app\1_ep.o
IF NOT EXIST flash\sys\app\2_sca.o set l_file=flash\sys\app\2_sca.o
IF NOT EXIST flash\sys\app\3_plc.o set l_file=flash\sys\app\3_plc.o
IF NOT EXIST flash\sys\app\4_cds.o set l_file=flash\sys\app\4_cds.o
IF NOT EXIST flash\sys\app\5_opt.o set l_file=flash\sys\app\5_opt.o
IF NOT EXIST flash\sys\starc\starc.zfx set l_file=flash\sys\starc\starc.zfx
IF NOT EXIST flash\sys\cds\hildpm3s.out set l_file=flash\sys\cds\hildpm3s.out
IF NOT EXIST flash\sys\cds\plcvxw.ini set l_file=flash\sys\cds\plcvxw.ini
IF NOT EXIST flash\sys\cds\plcvxw.o set l_file=flash\sys\cds\plcvxw.o
IF NOT EXIST flash\sys\cds\cdsiodrv.o set l_file=flash\sys\cds\cdsiodrv.o
IF NOT EXIST flash\sys\can\driver.o set l_file=flash\sys\can\driver.o
IF NOT EXIST flash\sys\can\canio.o set l_file=flash\sys\can\canio.o
IF NOT EXIST flash\sys\configs\system.zfx set l_file=flash\sys\configs\system.zfx
IF NOT EXIST flash\sys\configs\fontsize.cfx set l_file=flash\sys\configs\system.cfx
IF NOT EXIST flash\sys\configs\resources\english.cfx set l_file=flash\sys\configs\resources\english.cfx
IF NOT EXIST flash\sys\configs\resources\deutsch.cfx set l_file=flash\sys\configs\resources\deutsch.cfx
IF NOT EXIST flash\sys\configs\resources\francais.cfx set l_file=flash\sys\configs\resources\francais.cfx
IF NOT EXIST flash\sys\configs\resources\italiano.cfx set l_file=flash\sys\configs\resources\italiano.cfx
IF NOT EXIST flash\sys\configs\resources\espanol.cfx set l_file=flash\sys\configs\resources\espanol.cfx
IF NOT EXIST flash\sys\configs\resources\chinese.cfx set l_file=flash\sys\configs\resources\chinese.cfx
IF NOT EXIST flash\sys\configs\resources\japanese.cfx set l_file=flash\sys\configs\resources\japanese.cfx
IF NOT EXIST flash\sys\templates\versions.cfx set l_file=flash\sys\templates\versions.cfx
IF NOT EXIST flash\sys\templates\profile.cfx set l_file=flash\sys\templates\profile.cfx
IF NOT EXIST flash\sys\cifx\nxcif50-rte.bin set l_file=flash\sys\cifx\nxcif50-rte.bin
IF NOT EXIST flash\sys\cifx\fw\device.conf set l_file=flash\sys\cifx\fw\device.conf
IF NOT EXIST flash\sys\cifx\fw\channel0\cifxs3s.nxf set l_file=flash\sys\cifx\fw\channel0\cifxs3s.nxf
IF NOT EXIST flash\sys\cifx\fw\channel0\cifxecs.nxf set l_file=flash\sys\cifx\fw\channel0\cifxecs.nxf
IF NOT EXIST flash\sys\cifx\fw\channel0\cifxpls.nxf set l_file=flash\sys\cifx\fw\channel0\cifxpls.nxf
IF NOT EXIST flash\sys\uniVAL\sercos3.o set l_file=flash\sys\uniVAL\sercos3.o
IF NOT EXIST flash\sys\uniVAL\coe.o set l_file=flash\sys\uniVAL\coe.o
IF NOT EXIST flash\sys\uniVAL\powerlink.o set l_file=flash\sys\uniVAL\powerlink.o
IF NOT EXIST flash\sys\uniVAL\xdd\staubli_io.xdd set l_file=flash\sys\uniVAL\xdd\staubli_io.xdd
IF NOT EXIST flash\sys\uniVAL\xdd\unival_4p.xdd set l_file=flash\sys\uniVAL\xdd\unival_4p.xdd
IF NOT EXIST flash\sys\uniVAL\xdd\unival_5p.xdd set l_file=flash\sys\uniVAL\xdd\unival_5p.xdd
IF NOT EXIST flash\sys\uniVAL\xdd\unival_6p.xdd set l_file=flash\sys\uniVAL\xdd\unival_6p.xdd
IF NOT EXIST flash\usr\configs\templates\iomap\iomap.cfx set l_file=flash\usr\configs\templates\iomap\iomap.cfx
IF NOT EXIST flash\usr\recorder\records.cfx set l_file=flash\usr\recorder\records.cfx
IF NOT EXIST flash\usr\configs\profiles\default.cfx set l_file=flash\usr\configs\profiles\default.cfx
IF NOT EXIST flash\usr\configs\profiles\maintenance.cfx set l_file=flash\usr\configs\profiles\maintenance.cfx
IF NOT EXIST flash\usr\templates\default\default.pjx set l_file=flash\usr\templates\default\default.pjx
IF NOT EXIST flash\usr\templates\default\default.dtx set l_file=flash\usr\templates\default\default.dtx
IF NOT EXIST flash\usr\templates\default\start.pgx set l_file=flash\usr\templates\default\start.pgx
IF NOT EXIST flash\usr\templates\default\stop.pgx set l_file=flash\usr\templates\default\stop.pgx
IF NOT %l_file% == "" GOTO INVALID

REM Build directories needed for backup
IF NOT EXIST %l_temp%       MD %l_temp%
IF NOT EXIST %l_temp%	GOTO ERROR_TEMP
IF NOT EXIST %l_temp%\sys MD %l_temp%\sys
IF EXIST %l_temp%\options.cfx DEL %l_temp%\options.cfx

:TARGET
REM Compute target using bootline.dat if target is still unknown
IF NOT "%l_target%"=="" GOTO BOOTLINE
REM Retrieve current bootline.dat
IF EXIST %l_temp%\sys\bootline.dat DEL %l_temp%\sys\bootline.dat
type _ftpBootline.txt | _replaceStr #TEMP# %l_temp% | _replaceStr #LOGIN# %l_login% | _replaceStr #PASSWORD# %l_password% > %l_temp%\ftpBootline.txt
CALL ftp -i -n -v -s:%l_temp%\ftpBootline.txt %l_ipAdress% > %l_temp%\backup.txt
IF NOT EXIST %l_temp%\sys\bootline.dat GOTO ERROR_BOOTLINE
REM Download current bootline to determine target if target is still unknown
type %l_temp%\sys\bootline.dat | _replaceStr "ata=1,0(0,0):/ata0/" "CALL %l_temp%\bootline.bat %l_temp% " > %l_temp%\bootline2.bat
type _bootline.bat > %l_temp%\bootline.bat
CALL %l_temp%\bootline2.bat %l_temp%

:BOOTLINE
REM Create new bootline.dat unless 'keep' was requested
if "%l_keep%"=="true" GOTO FTP
REM Create the bootline.dat that matches the specified target
IF EXIST %l_temp%\sys\bootline.dat DEL %l_temp%\sys\bootline.dat
IF "%l_target%"=="men" ECHO ata=1,0(0,0):/ata0/men f=0xa tn=boot7 o=gei > %l_temp%\sys\bootline.dat
IF "%l_target%"=="men_hp" ECHO ata=1,0(0,0):/ata0/men_hp f=0xa tn=boot7 o=gei > %l_temp%\sys\bootline.dat
IF "%l_target%"=="men007" ECHO ata=1,0(0,0):/ata0/men007 f=0xa tn=boot7 o=fei > %l_temp%\sys\bootline.dat
IF "%l_target%"=="men07n" ECHO ata=1,0(0,0):/ata0/men07n f=0xa tn=boot7 o=fei > %l_temp%\sys\bootline.dat
IF "%l_target%"=="teknor" GOTO ERROR_TEKNOR

:FTP
REM Launch update script if bootline.dat file has been built successfully
IF NOT EXIST %l_temp%\sys\bootline.dat GOTO ERROR_BOOTLINE

ECHO.
ECHO  Use backup.bat command to first backup controller version.
ECHO  New files will be send to the target %l_ipAdress%.
ECHO  Press Ctrl+C to stop process.
PAUSE

rem First backup addons.
if not exist flash\sys\addons goto MAJ
    rem delete previous addons backup
    if exist %l_temp%\addons rd /s /q %l_temp%\addons
    md %l_temp%\addons
    md %l_temp%\addons\sys
    md %l_temp%\addons\usr
    md %l_temp%\addons\log
    rem Update ftp script
    for %%t in (sys usr log) do call :FTPAddonsBackup %%t

REM Update ftp script with target name
:MAJ
IF "%l_target%"=="" GOTO ERROR_BOOTLINE
IF NOT "%l_mode%"=="LLI" GOTO SRC
IF NOT "%l_target%"=="teknor" type _ftpCS8C_LLI.txt | _replaceStr #TEMP# %l_temp% | _replaceStr #LOGIN# %l_login% | _replaceStr #PASSWORD# %l_password% | _replaceStr #TARGET# %l_target% > %l_temp%\ftp.txt
GOTO CALL_FTP
:SRC
IF NOT "%l_target%"=="teknor" type _ftpCS8C_SRC.txt | _replaceStr #TEMP# %l_temp% | _replaceStr #LOGIN# %l_login% | _replaceStr #PASSWORD# %l_password% | _replaceStr #TARGET# %l_target% > %l_temp%\ftp.txt
:CALL_FTP
CALL ftp -i -v -n -s:%l_temp%\ftp.txt %l_ipAdress%

rem then update addons
if not exist flash\sys\addons goto WARNING
    rem Update ftp script
    echo user %l_login% %l_password% > %l_temp%\ftp.txt
    echo hash on >> %l_temp%\ftp.txt
    echo lcd flash\sys\addons >> %l_temp%\ftp.txt
    for %%t in (sys usr log) do call :FTPAddons %%t
    echo quit >> %l_temp%\ftp.txt
    CALL ftp -i -v -n -s:%l_temp%\ftp.txt %l_ipAdress%

:WARNING
ECHO.
ECHO --------------------------------------------------
ECHO  Please check that there was no network error during update before reboot.
ECHO --------------------------------------------------
ECHO.
GOTO END

:ERROR
ECHO.
ECHO ERROR IN PARAMETER. Format is :
ECHO.
ECHO      update mode ipAddress ftpLogin ftpPassword [target]
ECHO.
ECHO mode:   LLI / SRC
ECHO target: men (CS8C, CS8HP) / men007 (CS8, CS8C, CS8HP) / men07n (CS8, CS8C, CS8HP)
GOTO END

:ERROR_TEKNOR
ECHO.
ECHO ERROR Cannot install on CS8 with Teknor CPU board
PAUSE
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

:ERROR_BOOTLINE
ECHO.
ECHO ERROR: Unable to connect or retrieve target type.
ECHO ERROR: (network error, invalid login or password,
ECHO ERROR:  or /sys/bootline.dat missing or unexpected)
ECHO ERROR: Check network and login.
ECHO ERROR: If correct, specify a CPU type as 5th parameter.
GOTO END

:END
goto :eof

:FTPAddonsBackup
type _ftpAddonBackup.txt | _replaceStr #IP# %l_ipAdress% | _replaceStr #PART# %1 | _replaceStr #TEMP# %l_temp% | _replaceStr #LOGIN# %l_login% | _replaceStr #PASSWORD# %l_password% > %l_temp%\ftp.txt
CALL ftp -i -v -n -s:%l_temp%\ftp.txt %l_ipAdress%
goto :eof

:FTPAddons
echo cd /%1/app >> %l_temp%\ftp.txt
echo pwd >> %l_temp%\ftp.txt
rem for all known addons            if exist on backup                    update
if exist flash\sys\addons for %%a in (flash\sys\addons\*.o) do if exist %l_temp%\addons\%1\%%~nxa echo put %%~nxa >> %l_temp%\ftp.txt
goto :eof
