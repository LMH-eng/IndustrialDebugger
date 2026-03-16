# 编译说明

## 环境要求

- **Qt**: 5.15.2 或更高版本
- **CMake**: 3.16 或更高版本
- **编译器**: 
  - Windows: MinGW-w64 或 MSVC 2019+
  - Linux: GCC 7+ 或 Clang 6+

## Windows 编译步骤 (MinGW)

### 1. 安装 Qt

下载并安装 Qt 5.15.2：
- 访问 https://download.qt.io/archive/qt/5.15/5.15.2/
- 下载 `qt-opensource-windows-x86-5.15.2.exe`
- 安装时勾选 `MinGW 8.1.0 64-bit` 组件

### 2. 配置环境变量

添加到系统 PATH：
```
C:\Qt\5.15.2\mingw81_64\bin
C:\Qt\Tools\mingw810_64\bin
```

### 3. 编译项目

```bash
# 创建构建目录
mkdir build
cd build

# 配置 CMake（替换为你的 Qt 安装路径）
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/5.15.2/mingw81_64" ..

# 编译
mingw32-make

# 运行
.\bin\IndustrialDebugger.exe
```

## Linux 编译步骤

### 1. 安装依赖 (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install qt5-default qtbase5-dev qt5-qmake cmake build-essential
sudo apt-get install libqt5serialport5-dev
```

### 2. 编译项目

```bash
# 创建构建目录
mkdir build
cd build

# 配置 CMake
cmake ..

# 编译
make

# 运行
./bin/IndustrialDebugger
```

## 使用 Qt Creator 编译（推荐新手）

1. 打开 Qt Creator
2. 选择 `文件` -> `打开文件或项目`
3. 选择 `CMakeLists.txt` 文件
4. 配置项目（选择 Qt 版本）
5. 点击左下角的 `构建` 按钮
6. 点击 `运行` 按钮

## 常见问题

### 问题1：找不到 Qt

**错误信息**：
```
CMake Error: Could not find Qt5
```

**解决方法**：
- 确保 Qt 已正确安装
- 检查 CMAKE_PREFIX_PATH 是否正确
- 在 CMake 命令中指定 Qt 路径：
  ```bash
  cmake -DCMAKE_PREFIX_PATH="你的Qt安装路径" ..
  ```

### 问题2：找不到编译器

**错误信息**：
```
CMake Error: CMake was unable to find a build program corresponding to "MinGW Makefiles"
```

**解决方法**：
- 确保 MinGW 已安装并添加到 PATH
- 或者使用 MSVC 编译：
  ```bash
  cmake -G "Visual Studio 16 2019" ..
  ```

### 问题3：链接错误

**错误信息**：
```
undefined reference to `SerialPortManager::xxx'
```

**解决方法**：
- 确保 Qt SerialPort 模块已安装
- 检查 CMakeLists.txt 中的 Qt 模块链接

## 验证安装

编译成功后，运行程序应该看到：
- 主窗口正常显示
- 网络配置区（TCP客户端/服务端、UDP）
- 串口配置区（串口选择、波特率等）
- 数据收发区（发送框、格式选择）
- 日志显示区（接收数据、时间戳）

## 下一步

编译成功后，可以：
1. 测试 TCP 通信（启动两个实例互相通信）
2. 测试串口通信（如果有串口设备或虚拟串口工具）
3. 根据需求添加新功能
