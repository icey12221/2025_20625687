cmake_minimum_required(VERSION 3.19)
project(Worksheet6 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.2/msvc2022_64")
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Worksheet6
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    icons.qrc
    partdialog.h
    partdialog.cpp
    partdialog.ui
)

target_link_libraries(Worksheet6
    PRIVATE
        Qt6::Core
        Qt6::Widgets
)

include(GNUInstallDirs)

install(TARGETS Worksheet6
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Worksheet6
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
