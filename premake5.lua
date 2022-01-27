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

    files { "src/Sandbox/**.h", "src/Sandbox/**.cpp", "src/Sandbox/**.hpp" }
    includedirs { "src/Sandbox", "src" }
    
    links { "CppHashMap" }

    filter "platforms:x64"
        architecture "x64"
        

project "UnitTest"

    kind "ConsoleApp"
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"
    
    files { "src/UnitTest/**.h", "src/UnitTest/**.cpp", "src/UnitTest/**.hpp" }
    includedirs { "src/UnitTest", "src" }

    links { "CppHashMap" }

    filter "platforms:x64"
        architecture "x64"