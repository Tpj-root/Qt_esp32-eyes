cmake_minimum_required(VERSION 3.16)
project(Esp32EyesQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(LIB_SRC
    AsyncTimer.cpp
    BlinkAssistant.cpp
    Eye.cpp
    EyeBlink.cpp
    EyeTransformation.cpp
    EyeTransition.cpp
    EyeVariation.cpp
    Face.cpp
    FaceBehavior.cpp
    FaceExpression.cpp
    LookAssistant.cpp
)

add_executable(esp32-eyes-qt
    ${LIB_SRC}
    main.cpp
    MainWindow.cpp
    FaceWidget.cpp
)

target_include_directories(esp32-eyes-qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(esp32-eyes-qt PRIVATE Qt6::Widgets)