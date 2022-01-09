workspace "CppHashMap"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    location "build"
    startproject "Sandbox"


project "CppHashMap"

    kind "StaticLib"
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    includedirs { "src/CppHashMap" }
    files { "src/CppHashMap/**.h", "src/CppHashMap/**.cpp", "src/CppHashMap/**.hpp" }
    filter "platforms:x64"
        architecture "x64"

    
project "Sandbox"

    kind "ConsoleApp" -- Distinction from a "windowed app" is important apparently
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    includedirs { "src/Sandbox", "src" }

    files { "src/Sandbox/**.h", "src/Sandbox/**.cpp", "src/Sandbox/**.hpp" }
    
    links { "CppHashMap" }

    filter "platforms:x64"
        architecture "x64"