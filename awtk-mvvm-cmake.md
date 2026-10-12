# AWTK MVVM CMake 构建说明

awtk-mvvm 支持 CMake 与 SCons 两套构建，二者并存。CMake 需 3.16 及以上。

> CMake 版依赖**已安装**的 AWTK，请先使用 CMake 完成 AWTK 的编译与安装。

## 编译与安装

需要先使用 gen.sh 生成 mvvm.def(需要 node):

```bash
cd awtk-mvvm
./gen.sh
cmake -S . -B build_cmake -DCMAKE_PREFIX_PATH=/path/to/awtk-install
cmake --build build_cmake --parallel
cmake --install build_cmake --prefix ../awtk-mvvm-install
```

`CMAKE_PREFIX_PATH` 须指向 AWTK 的安装目录。

Visual Studio、Xcode 这类多配置生成器，需在 build 和 install 时补 `--config Release` / `--config Debug` 或者其他的配置。

## 在自己的工程中使用

在 `CMakeLists.txt` 中：

```cmake
find_package(awtk-mvvm CONFIG REQUIRED)
add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE awtk_mvvm::mvvm)
```

执行cmake时，`CMAKE_PREFIX_PATH` 需同时包含 awtk-mvvm 与 AWTK 两个安装前缀（用分号分隔）。

```bash
cd /path/to/your_app
cmake -S . -B build_cmake -DCMAKE_PREFIX_PATH="/path/to/awtk-install;/path/to/awtk-mvvm-install"
```

> 在 awtk-mvvm 安装目录 `lib/cmake/awtk-mvvm/` 下有 `awtk-mvvmConfig.cmake`、`awtk-mvvmTargets.cmake`；头文件在 `include/mvvm/` 下。
>

## 与 SCons 的差异

**CMake 版不含 JS 支持。** `jerryscript` 与 `iotjs` 在 CMake 中尚未实现，相关选项默认关闭且打开会直接报错：

| 选项 | 说明 |
|---|---|
| `AWTK_MVVM_WITH_JERRYSCRIPT` | 未实现，须为 OFF |
| `AWTK_MVVM_WITH_IOTJS` | 未实现，须为 OFF |

需要 JS 绑定（`src/mvvm/jerryscript/`、`src/mvvm/iotjs/`）请使用 SCons。

**CMake 版不构建 demos。** 根 `CMakeLists.txt` 只定义 `mvvm` 一个库目标，demo 只有 SCons 一份构建脚本。需要 demo 请使用 SCons。
