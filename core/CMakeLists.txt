# core 作为独立库编译

add_library(MediatorCore
    # policy
    policy/ISubscriptionPolicy.h
    policy/AlwaysPolicy.h
    policy/ValueChangedPolicy.h
    policy/StickyPolicy.h

    # mediator
    mediator/Observe.h
    mediator/Observe.cpp
    mediator/Topic.h
    mediator/Topic.cpp
    mediator/Mediator.h
    mediator/Mediator.cpp

    # actor
    actor/ActorObserve.h
    actor/ActorObserve.cpp

    # binding (依赖 QtWidgets)
    binding/Binding.h
    binding/BindingHelper.h

    # view
    view/LogListView.h
    view/LogListView.cpp
    view/LogListView.ui

    # viewmodel
    viewmodel/BaseViewModel.h
    viewmodel/LogListViewModel.h
    viewmodel/LogListViewModel.cpp

    # model
    model/BaseModel.h
    model/BaseModel.cpp
    model/LogModel.h
    model/LogModel.cpp
    model/LogEntry.h
    model/IModel.h
)

target_include_directories(MediatorCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}      # core/
        ${CMAKE_CURRENT_SOURCE_DIR}/..   # 项目根（兼容 include "core/..."）
)

target_link_libraries(MediatorCore
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets   # 你 core 里还有 binding，所以需要 Widgets
        Qt${QT_VERSION_MAJOR}::Sql       # LogModel 需要 QtSql
)

target_compile_features(MediatorCore PUBLIC cxx_std_17)

add_library(MediatorCore::MediatorCore ALIAS MediatorCore)

# ========== install 命令 ==========

# 安装头文件（递归）
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/MediatorCore
    FILES_MATCHING PATTERN "*.h"
    PATTERN "CMakeLists.txt" EXCLUDE
    PATTERN "*.cpp" EXCLUDE
    PATTERN "build" EXCLUDE
)

# 安装库文件
install(TARGETS MediatorCore
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

# 生成并安装 CMake 配置
include(CMakePackageConfigHelpers)

# Config 文件
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/MediatorCoreConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MediatorCoreConfig.cmake
    INSTALL_DESTINATION lib/cmake/MediatorCore
)

# Version 文件
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MediatorCoreConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MediatorCoreConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MediatorCoreConfigVersion.cmake
    DESTINATION lib/cmake/MediatorCore
)
