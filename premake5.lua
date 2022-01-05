workspace "CppHashMap"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    location "build"

project "CppHashMap"

    kind "ConsoleApp" -- Distinction from a "windowed app" is important apparently
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "**.h", "**.cpp", "*.hpp" }

    filter "platforms:x64"
        architecture "x64"