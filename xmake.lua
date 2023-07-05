set_project("roamer-engine")

set_languages("c++23")
set_warnings("all", "error")

if is_plat("windows") then
    add_cxflags("/wd4819") -- Add this if using msvc to ignore encoding issues
    add_cxxflags("/MP")
end

add_rules("mode.debug", "mode.release")
add_rules("c.unity_build")
add_rules("c++.unity_build")

set_runtimes("MD")

add_requireconfs("*", {configs = {shared = true}})
add_requires("glad")
add_requires("glfw")
add_requires("glm")
add_requires("vcpkg::soil2", {alias = "soil2"})
add_requires("stb")
add_requires("tinyobjloader")
add_requires("imgui", {configs = {glfw = true, opengl3 = true}})
add_requires("imgui-sugar")

set_rundir("output")
before_run(function () 
    os.mkdir("output")
end)

target("roamer-engine")
    set_kind("shared")
    add_rules("utils.symbols.export_all", {export_classes = true})
    add_includedirs("RoamerEngine/include", {public = true})
    add_files("RoamerEngine/src/**/*.cpp")
    add_packages("glad", "glfw", "glm", "soil2", "stb", "tinyobjloader")
    add_links("opengl32")

target("roamer-editor")
    set_kind("shared")
    add_rules("utils.symbols.export_all", {export_classes = true})
    add_deps("roamer-engine")
    add_includedirs("RoamerEditor/include", {public = true})
    add_files("RoamerEditor/src/**/*.cpp")
    add_packages("glfw", "imgui", "imgui-sugar")

target("example-beginner")
    add_deps("roamer-editor")
    add_files("examples/beginner/main.cpp")
    add_packages("glad", "imgui-sugar")
    set_rundir("examples/beginner")

package("imgui-sugar")
    add_urls("https://github.com/QuadnucYard/imgui_sugar/archive/refs/tags/v1.0.5-fix.tar.gz")
    add_versions("1.0.5-fix", "af35a8c93a1412214558cfc60a9e4cb44b827676d422658a653970fb58d6e8d3")

    on_install("macosx", "linux", "windows", "mingw", "android", "iphoneos", function (package)
        io.writefile("xmake.lua", [[
            target("imgui-sugar")
                set_kind("headeronly")
                add_includedirs(".")
                add_headerfiles("imgui_sugar.hpp")
        ]])
        import("package.tools.xmake").install(package)
    end)
package_end()


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

