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
    include/common/WordType.hpp
    include/common/WordImage.hpp

    include/storage/WordStorage.hpp
    include/db/WordDao.hpp
    include/net/WordParser.hpp
    include/net/WordService.hpp

    include/model/WordModel.hpp

    include/util/Error.hpp
    include/util/Result.hpp
    include/util/EnumHelper.hpp
)

set(SOURCES
    main.cpp

    src/storage/WordStorage.cpp
    src/db/WordDao.cpp
    src/net/WordParser.cpp
    src/net/WordService.cpp

    src/model/WordModel.cpp
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

set(QGUMBO_PARSER_SRC libs/QGumboParser)
set(LIBQGUMBO_PARSER_PATH ${CMAKE_BINARY_DIR}/libs/QGumboParser/libQGumboParser.so)

#add_subdirectory(${QGUMBO_PARSER_SRC})
add_library(QGumboParser SHARED IMPORTED)
set_target_properties(QGumboParser PROPERTIES IMPORTED_LOCATION ${LIBQGUMBO_PARSER_PATH})

add_executable(${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/${QGUMBO_PARSER_SRC}
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Sql
    Qt6::Network
    Qt6::Quick

    QGumboParser
)
