set_project("roamer-engine")

set_languages("c++23")
set_warnings("all", "error")

if is_plat("windows") then
    -- add_cxflags("/wd4819") -- Add this if using msvc
    -- add_ldflags("/PROFILE")
else
    -- add_cxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")
    -- add_mxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=expansion-to-defined")
end

add_rules("mode.debug", "mode.release")
-- add_rules("c.unity_build")
-- add_rules("c++.unity_build")

set_runtimes("MD")

add_requireconfs("*", {configs = {shared = true}})
add_requires("vcpkg::glad[loader]", {alias = "glad"})
add_requires("vcpkg::glfw3", {alias = "glfw3"})
add_requires("vcpkg::glm", {alias = "glm"})
add_requires("vcpkg::soil2", {alias = "soil2"})
add_requires("vcpkg::stb", {alias = "stb"})
add_requires("vcpkg::tinyobjloader", {alias = "tinyobjloader"})
add_requires("vcpkg::imgui[docking-experimental,glfw-binding,opengl3-binding]", {alias = "imgui"})

set_rundir("output")
before_run(function () 
    os.mkdir("output")
end)

target("roamer-engine")
    set_kind("static")
    add_includedirs("RoamerEngine/include", {public = true})
    add_files("RoamerEngine/src/**/*.cpp")
    add_packages("glad", "glfw3", "glm", "soil2", "stb", "tinyobjloader")
    add_links("opengl32")

target("roamer-editor")
    set_kind("static")
    add_deps("roamer-engine")
    add_includedirs("RoamerEditor/include", {public = true})
    add_files("RoamerEditor/src/**/*.cpp")
    add_packages("imgui")

target("example-beginner")
    add_deps("roamer-editor")
    add_files("examples/beginner/main.cpp")
    add_packages("glad", "glfw3", "glm", "soil2", "stb", "tinyobjloader")
    set_rundir("examples/beginner")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

