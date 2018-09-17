-- Create Solution

solution "AwesomeEngineSolution"
configurations{"Debug","Release"}
location("build")
targetdir("data")



project "AwesomeEngine"
	kind "ConsoleApp"
	language "C++"
	objdir "build/AwesomeEngine"
	defines {"_GLFW_WIN32","_GLFW_WGL","_GLFW_USE_OPENGL","GLEW_STATIC","WINDOWS_PLATFORM"}
	libdirs{"lib"}
	links{"opengl32"}
	includedirs{"include","deps"}
	files{"include/**.h",
		  "include/**.hpp",
		  "src/**.cc",
		  "src/**.c",
		  "src/**.cpp",
		  "src/**.h",
		  }
	files{"examples/main.cc"}
	debugdir("data")
	configuration "Debug"
	flags "Symbols"

project "AwesomeEngineBuild"
	kind "StaticLib"
	language "C++"
	targetdir("lib")
	objdir "build/AwesomeEngineBuild"
	defines {"_GLFW_WIN32","_GLFW_WGL","_GLFW_USE_OPENGL","GLEW_STATIC","WINDOWS_PLATFORM"}
	links{"opengl32"}
	includedirs{"include","deps"}
	files{"include/**.h",
	  "deps/**.h",
	  "deps/**.hpp",
	  "src/**.cc",
	  "src/**.c",
	  "src/**.cpp",
	  "src/**.h",
	  }
	configuration ("Debug")
	flags "Symbols"



project "AwesomeEngineTest"
	kind "ConsoleApp"
	language "C++"
	objdir "build/AwesomeEngineTest"
	libdirs{"lib"}
	links{"opengl32","AwesomeEngineBuild"}
	defines {"_GLFW_WIN32","_GLFW_WGL","_GLFW_USE_OPENGL","GLEW_STATIC","WINDOWS_PLATFORM"}
	includedirs{"include","deps"}
	files{"include/**.h"}
	files{"examples/main.cc"}
    debugdir("data")
	configuration ("Debug")
	flags "Symbols"






