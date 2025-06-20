workspace "Hazel"
	architecture "x64"

	configuration
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Hazel"
	location "Hazel"
	kind "sharedLib"
	language "C++"

	targetdir("bin/" ..outputdir .. "/%{prj.name}")
	objdir("bin-int/" ..outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	include
	{
		"%{prj.name}/src/vendor/spdlog/include"
	}

	filter "system:windows"
		