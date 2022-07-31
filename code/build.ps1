
if(!(Test-Path("../bin")))
{
        New-Item -Itemtype Directory "../bin"
}

$source_name = "../code/main.cpp","../code/SDL_c.cpp","../code/game.cpp"
$executable_name = "skidoo.exe"
$executable_name_linux = "skidoo.o"
$lib_path ="../lib/"
$include_path ="../include"

$compiler_flags = "/nologo","/EHsc","/Zi" ,"/FC"
$gcc_flags="-EHsc","-g"
# $linker_flags =
$libraries = "SDL2main.lib","SDL2.lib", "user32.lib", "shell32.lib", "gdi32.lib"
Push-Location ..\bin
if(!(Test-Path("SDL2.dll")))
{
        Copy-Item "../lib/SDL2.dll"
}

# cl /MD $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries $linker_flags /SUBSYSTEM:console
# cl $source_name /Fe$executable_name $compiler_flag
# clang -g  -I..\include\  -L..\lib\ -l $libraries  $source_name -o $executable_name $compiler_flags
#if(Get-Command cl){
#cl  $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries /SUBSYSTEM:CONSOLE
#}
if(Get-Command g++){
g++ $source_name -o $executable_name_linux -lSDL2 
}