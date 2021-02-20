set tools_path=%1
set project_path=%2
set cef_version=cef_binary_88.2.9+g5c8711a+chromium-88.0.4324.182_windows64

cd %tools_path%
if not errorlevel 0 goto :EOF

if not exist "%cef_version%.tar.bz2" (
	curl.exe -o %cef_version%.tar.bz2 "https://cef-builds.spotifycdn.com/%cef_version%.tar.bz2"
	if not errorlevel 0 goto :EOF
)

7z.exe x %cef_version%.tar.bz2 -aoa
if not errorlevel 0 goto :EOF

7z.exe x %cef_version%.tar -aoa
if not errorlevel 0 goto :EOF

robocopy /e %cef_version%  %project_path%/External/cef/windows-bin
if not errorlevel 1 goto :EOF & rem 1 - is success code of robocopy

cd %project_path%/External/cef/windows-bin
if not errorlevel 0 goto :EOF

cmake -G"Visual Studio 16 2019"
if not errorlevel 0 goto :EOF

msbuild.exe libcef_dll_wrapper\libcef_dll_wrapper.vcxproj /property:Configuration=Release /property:Platform=x64
msbuild.exe libcef_dll_wrapper\libcef_dll_wrapper.vcxproj /property:Configuration=Debug /property:Platform=x64
