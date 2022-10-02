# CGProjects

计算机图形学大作业。内置从零搭建的引擎 $\texttt{RoamerEngine}$，模仿了 Unity 风格。

## 环境要求

- VS2022 (Support for c++20)
- vcpkg (Used for package management)

## Dependencies

请注意配置 vcpkg，建议加到环境变量

### vcpkg list

```text
glew:x64-windows                                  2.2.0
glfw3:x64-windows                                 3.3.8
glm:x64-windows                                   0.9.9.8#1
opengl:x64-windows                                2022-03-14#1
soil2:x64-windows                                 1.3.0 
```

### Install

```sh
vcpkg install glew:x64-windows glfw3:x64-windows glm:x64-windows
vcpkg integrate install
```

其他库类似安装，在 `install` 后面写所有要安装的包。也可以一次安装一个。  
使用 `vcpkg integrate install` 进行全局配置。  

### dll import

从 `vcpkg/packages/...` 下，  
将 `glew32d.dll`, `glfw3.dll` 复制到 `(root)/x64/Debug`。  
将 `glew32.dll`, `glfw3.dll` 复制到 `(root)/x64/Release`。  
注意 Debug 版要从包的 `debug` 目录下复制。  
