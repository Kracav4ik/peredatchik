cmake_minimum_required(VERSION 3.16)
project(peredatchik)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_CXX_STANDARD 20)

set(SOURCES_COMMON
        BasePanel.cpp
        BasePanel.h
        colors.h
        EditableText.cpp
        EditableText.h
        LineEdit.cpp
        LineEdit.h
        Lines.cpp
        Lines.h
        MessagePopup.cpp
        MessagePopup.h
        Popup.cpp
        Popup.h
        priemoperedacha.cpp
        priemoperedacha.h
        Screen.cpp
        Screen.h
        utils.cpp
        utils.h
)

set(SOURCES_PRIEM
        main-priem.cpp
        PriemPanel.cpp
        PriemPanel.h
)

set(SOURCES_PEREDACHA
        main-peredacha.cpp
        PeredachaPanel.cpp
        PeredachaPanel.h
)

add_executable(priemnik ${SOURCES_PRIEM} ${SOURCES_COMMON})
add_executable(peredatchik ${SOURCES_PEREDACHA} ${SOURCES_COMMON})
