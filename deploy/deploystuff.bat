::change according to your installation

set projectPath=D:\Project\SNESTilesKitten\
set compilePath=D:\Project\compile\STK\
set deployPath=D:\Project\SNESTilesKitten\deploy\SNESTilesKitten\
set originalBinDir=%compilePath%
set vscdll=D:\Visual Studio\VC\Redist\MSVC\14.12.25810\x64\Microsoft.VC141.CRT\msvcp140.dll

rmdir /Q /S %deployPath%
mkdir %deployPath%
:: Compile

::D:\Visual Studio\VC\vcvarsall.bat amd64
:: D:\Visual Studio\VC\Auxiliary\Build\vcvarsall.bat amd64

mkdir %compilePath%
cd %compilePath%
set QMAKE_MSC_VER=1910
qmake %projectPath%\SNESTilesKitten.pro -spec win32-msvc "CONFIG+=release"
nmake


:: Copy file
mkdir %deployPath%\plugins\
xcopy /y %originalBinDir%\plugins\nintendocompression.dll %deployPath%\plugins\
xcopy /y %originalBinDir%\plugins\terranigmacompression.dll %deployPath%\plugins\

xcopy /y %originalBinDir%\release\SNESTilesKitten.exe %deployPath%
xcopy /y %originalBinDir%\SNESTinyKitten\release\SNESTinyKitten.exe %deployPath%

mkdir %deployPath%\Presets
mkdir %deployPath%\TilesPatterns
xcopy /s %projectPath% %deployPath%\Presets
xcopy /s %projectPath% %deployPath%\TilesPatterns

windeployqt.exe --no-translations --no-system-d3d-compiler --no-opengl --no-svg --no-webkit --no-webkit2 --release %deployPath%\SNESTilesKitten.exe

:: Clean up Qt extra stuff
rmdir /Q /S %deployPath%\imageformats
del %deployPath%\opengl32sw.dll
del %deployPath%\libEGL.dll
del %deployPath%\libGLESV2.dll

xcopy /y "%vscdll%" %deployPath%
