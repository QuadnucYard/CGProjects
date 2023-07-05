# CGProjects

计算机图形学大作业。内置从零搭建的引擎 $\texttt{RoamerEngine}$，模仿了 Unity 风格。

基于 ImGui 的编辑器 $\texttt{RoamerEditor}$ 在开发中。

## Development Environment

感谢 XMake 提供的构建技术支持！现在已经不需要 VS 环境来编译了。但您仍然需要 MSVC 的 `/std:c++lastest` 进行编译。

默认改用 xrepo 作为包管理器。为了重复利用之前 vcpkg 装的包，新版仍然可以用 vcpkg 做包管理器。没有被 vcpkg 收录的 `imgui-sugar`（存在 include 路径问题）也能直接作为自定义包安装了。所有依赖管理均由 XMake 完成，无效额外安装依赖。

如果要使用 Visual Studio 开发，可以通过输入以下命令生成解决方案：

```sh
xmake project -k vsxmake -m "debug,release"
```

### Linking

#### For vcpkg

include 和 lib 不用管，会自动搜索和链接，除了高贵的 `glfw3`。它的静态库名叫 `glfw3dll.lib`，链接时会提示找不到 `glfw3.lib`，因此需要手动改名复制一份。此外需要把 `glfw3.dll` 挪到输出目录，否则运行程序时会出异常。

#### For xrepo

除了 soil2，其他包均可由 xrepo 提供。您也不再需要解决高贵的 glfw3 问题。
