cmake_minimum_required(VERSION 3.22.1)

project(grunwald LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -Wall")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Sql
    Network
    Quick
)

set(HEADERS
    include/common/Word.hpp

    include/db/WordDao.hpp
    include/net/WordParser.hpp
    include/net/DictionaryService.hpp

    include/util/Result.hpp
)

set(SOURCES
    main.cpp

    src/db/WordDao.cpp
    src/net/WordParser.cpp
    src/net/DictionaryService.cpp
)

set(RESOURCES
     main.qrc
)

list(APPEND QML_IMPORT_PATH "qml")
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    CACHE STRING "Qt Creator extra qml import paths"
    FORCE
)

add_executable(${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Sql
    Qt6::Network
    Qt6::Quick
)
