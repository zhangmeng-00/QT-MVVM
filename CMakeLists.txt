cmake_minimum_required(VERSION 3.16)
project(MediatorMVAQt LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 统一用 C++17（你原来被后面的 C++14 覆盖了）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt5/Qt6 自适应
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql Network SerialBus)

if (MSVC)
    # 解决中文注释 / 字符串在 MSVC 下的编码问题
    add_compile_options(/utf-8)
endif()

# 1) 先编译 core 库
add_subdirectory(core)

# 2) 再编译应用程序
add_executable(${PROJECT_NAME}
    main.cpp
    MainWindow.h
    MainWindow.cpp
    MainWindow.ui

    common/TypeKey.h

    view/TraceViewer.h
    view/TraceViewer.cpp
    view/LogListView.h
    view/LogListView.cpp
    view/LogListView.ui

    viewmodel/BaseViewModel.h
    viewmodel/UserViewModel.h
    viewmodel/UserViewModel.cpp
    viewmodel/SensorViewModel.h
    viewmodel/SensorViewModel.cpp
    viewmodel/LogListViewModel.h
    viewmodel/LogListViewModel.cpp

    model/BaseModel.h
    model/BaseModel.cpp
    model/SensorModel.h
    model/SensorModel.cpp
    model/UserModel.h
    model/UserModel.cpp
    model/LoggerActor.h
    model/RecorderActor.h
    model/SQLiteRecorderActor.h
    model/SQLiteRecorderActor.cpp
    model/LogEntry.h
    model/LogModel.h
    model/LogModel.cpp

    model/modbus/ModbusMasterModel.cpp
    model/modbus/ModbusMasterModel.h
    model/modbus/ModbusModuleBase.cpp
    model/modbus/ModbusModuleBase.h
    model/modbus/PidPressureModule.cpp
    model/modbus/PidPressureModule.h
    model/modbus/PointDef.h
    app/AppContext.h
    app/AppContext.cpp
    app/Bootstrap.h
    app/Bootstrap.cpp
    common/TestStruct.h
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MediatorCore
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::SerialBus
)
