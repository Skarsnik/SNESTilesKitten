::change according to your installation

set projectPath=F:\Project\SNESTilesKitten\
set compilePath=F:\Project\compile\STK\
set deployPath=F:\Project\SNESTilesKitten\deploy\SNESTilesKitten\
set originalBinDir=%compilePath%

rmdir /Q /S %deployPath%
mkdir %deployPath%
:: Compile

::D:\Visual Studio\VC\vcvarsall.bat amd64
:: D:\Visual Studio\VC\Auxiliary\Build\vcvarsall.bat amd64

mkdir %compilePath%
cd %compilePath%
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
