
if(!(Test-Path("../bin")))
{
        New-Item -Itemtype Directory "../bin"
}

$source_name = "../code/main.cpp","../code/SDL_classes.cpp","../code/game.cpp","../code/line.cpp"
$executable_name = "skidoo.exe"
$executable_name_linux = "skidoo.o"
if(Test-Path("C:/vcpkg/vcpkg"))
{
$lib_path ="C:/vcpkg/vcpkg/installed/x64-windows/lib/"
$include_path ="C:/vcpkg/vcpkg/installed/x64-windows/include/"
	
}
else
{
	$lib_path="../lib"
	$include_path="../include"
}

$compiler_flags = "/nologo","/EHsc","/Zi" ,"/FC"
$gcc_flags="-g"
$msvc_flags="/EHsc"
$windows_libraries = "SDL2main.lib","SDL2.lib","SDL2_mixer.lib", "user32.lib", "shell32.lib", "gdi32.lib"
$linux_linker_flags= "-lSDL2", "-lSDL2_mixer"
Push-Location ../bin


#if(Get-Command cl){
#cl  $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries /SUBSYSTEM:CONSOLE
#}
if(Get-Command g++){
##+ $source_name -o $executable_name_linux -lSDL2 
#
#cl  $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries /SUBSYSTEM:CONSOLE
g++ $source_name -o $executable_name_linux $linux_linker_flags  $gcc_flags
cd ..
./bin/skidoo.o
}
if(Get-Command cl)
{
#	
#if(!(Test-Path("SDL2.dll")))
#{
#      Copy-Item "../lib/SDL2.dll"
#}

#if(!(Test-Path("SDL2_mixer.dll")))
#{
 #     Copy-Item "../lib/SDL2_mixer.dll"
#}

	cl  $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $windows_libraries /SUBSYSTEM:CONSOLE /MACHINE:x64 /NODEFAULTLIB:msvcrt.lib
	cd ..
	./bin/skidoo.exe
}
