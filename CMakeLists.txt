cmake_minimum_required(VERSION 3.16)
project(MediatorMVAQt LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network SerialBus)

if (MSVC)
    add_compile_options(/utf-8)
endif()



add_executable(${PROJECT_NAME}
    main.cpp
    MainWindow.h
    MainWindow.cpp
    MainWindow.ui






    core/policy/ISubscriptionPolicy.h
    core/policy/AlwaysPolicy.h
    core/policy/ValueChangedPolicy.h

    core/mediator/Observe.h
    core/mediator/Observe.cpp
    core/mediator/Topic.h
    core/mediator/Topic.cpp
    core/mediator/Mediator.h
    core/mediator/Mediator.cpp
    core/mediator/TopicMeta.h
    core/mediator/TopicRegistry.h
    core/mediator/TopicRegistry.cpp

    core/actor/ActorObserve.h
    core/actor/ActorObserve.cpp

    core/command/ICommand.h
    core/command/SimpleCommand.h

    core/binding/BindingCommand.h
    core/binding/Binding.h
    core/binding/BindingHelper.h
    core/app/AppContext.cpp
    core/app/AppContext.h


    common/TypeKey.h






    view/TraceViewer.h
    view/TraceViewer.cpp
    viewmodel/SensorViewModel.cpp


    viewmodel/BaseViewModel.h
    viewmodel/UserViewModel.h
    viewmodel/UserViewModel.cpp
    viewmodel/SensorViewModel.h



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



    model/modbus/ModbusMasterModel.cpp
    model/modbus/ModbusMasterModel.h
    model/modbus/ModbusModuleBase.cpp
    model/modbus/ModbusModuleBase.h
    model/modbus/PidPressureModule.cpp
    model/modbus/PidPressureModule.h
    model/modbus/PointDef.h
    core/app/Bootstrap.h
    core/app/Bootstrap.cpp
    core/command/DispatcherCommand.h
    core/binding/BindingCommandHelpers.h



)

target_include_directories(MediatorMVAQt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
)


target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt5::Widgets
    Qt5::Sql
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::SerialBus)
