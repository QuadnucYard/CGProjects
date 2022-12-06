# CGProjects

计算机图形学大作业。内置从零搭建的引擎 $\texttt{RoamerEngine}$，模仿了 Unity 风格。

基于 ImGui 的编辑器 $\texttt{RoamerEditor}$ 在开发中。

## Development Environment

- VS2022 (Requires `/std:c++lastest` to support c++23)
- vcpkg (Used for package management)

## Dependencies

请注意配置 vcpkg，建议加到环境变量。

### Packages Used

安装的包如下。部分包由于被依赖而安装，在此没列出。其中 ImGui 用于编辑器，引擎本体不需要。

```text
glad, glfw3, glm, soil2, tinyobjloader
imgui[glfw-binding,opengl3-binding,docking-experimental]
```

额外：用于 ImGui 的语法糖。[https://github.com/mnesarco/imgui_sugar](https://github.com/mnesarco/imgui_sugar)。因为没被 vcpkg 收录，需要手动下载（单头文件），放到 include 目录中。

### vcpkg list

```text
glad:x64-windows                                  0.1.36              Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader...
glad[loader]:x64-windows                                              Generate loader logic.
glfw3:x64-windows                                 3.3.8               GLFW is a free, Open Source, multi-platform libr...
glm:x64-windows                                   0.9.9.8#1           OpenGL Mathematics (GLM)
imgui:x64-windows                                 1.88#1              Bloat-free Immediate Mode Graphical User interfa...
imgui[docking-experimental]:x64-windows                               Build with docking support
imgui[glfw-binding]:x64-windows                                       Make available GLFW binding
imgui[opengl3-binding]:x64-windows                                    Make available OpenGL3/ES/ES2 (modern) binding
soil2:x64-windows                                 1.3.0               Simple OpenGL Image Library 2
stb:x64-windows                                   2021-09-10#1        public domain header-only libraries
tinyobjloader:x64-windows                         2.0.0-rc9           Tiny but powerful single file wavefront obj loader
```

### Install Example

```sh
vcpkg install glfw3:x64-windows glm:x64-windows
vcpkg install imgui[glfw-binding,opengl3-binding,docking-experimental]:x64-windows 
vcpkg integrate install
```

其他库类似安装，在 `install` 后面写所有要安装的包。也可以一次安装一个。
注意 Editor 使用的 ImGui 要安装 2 个 binding 和 docking。
如果某个包未完全安装，重新安装请加上 `--recurse` 参数（有命令行提示）。

最后使用 `vcpkg integrate install` 进行全局配置。  

### Import

include 和 lib 不用管，会自动搜索和链接。

dll 需要从 `vcpkg\installed\x64-windows\bin`，将用到的 dll 复制到 `(root)/x64/Release`。从 `vcpkg\installed\x64-windows\debug\bin`，将调试模式需要的 dll 复制到 `(root)/x64/Debug`。

目前用到的 dll 只有 `glfw3.dll`。
