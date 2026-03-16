cmake_minimum_required(VERSION 3.16)
project(IndustrialDebugger VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt模块
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    SerialPort
)

# 源文件
set(SOURCES
    src/main.cpp
    src/MainWindow/MainWindow.cpp
    src/Network/NetworkManager.cpp
    src/Network/TcpClient.cpp
    src/Network/TcpServer.cpp
    src/Network/UdpClient.cpp
    src/SerialPort/SerialPortManager.cpp
    src/DataProcessing/DataProcessor.cpp
    src/DataProcessing/LogManager.cpp
)

# 头文件
set(HEADERS
    src/MainWindow/MainWindow.h
    src/Network/NetworkManager.h
    src/Network/TcpClient.h
    src/Network/TcpServer.h
    src/Network/UdpClient.h
    src/SerialPort/SerialPortManager.h
    src/DataProcessing/DataProcessor.h
    src/DataProcessing/LogManager.h
)

# UI文件
set(FORMS
    src/MainWindow/MainWindow.ui
)

# 创建可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
)

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    Qt5::SerialPort
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Windows下设置应用图标（可选）
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
