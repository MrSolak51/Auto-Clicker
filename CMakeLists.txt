cmake_minimum_required(VERSION 3.10)
project(AccountantManager LANGUAGES CXX)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
find_library(MYSQLCLIENT_LIB mysqlclient)
if(NOT MYSQLCLIENT_LIB)
    message(FATAL_ERROR "mysqlclient library not found")
endif()

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/account.cpp
    ${PROJECT_SOURCE_DIR}/src/account_gui.cpp
    ${PROJECT_SOURCE_DIR}/src/boxes.cpp
    ${PROJECT_SOURCE_DIR}/src/database.cpp
    ${PROJECT_SOURCE_DIR}/src/date_picker.cpp
    ${PROJECT_SOURCE_DIR}/src/dialog.cpp
    ${PROJECT_SOURCE_DIR}/src/graph.cpp
    ${PROJECT_SOURCE_DIR}/src/gui.cpp
    ${PROJECT_SOURCE_DIR}/src/list.cpp
    ${PROJECT_SOURCE_DIR}/src/login.cpp
    ${PROJECT_SOURCE_DIR}/src/node.cpp
)

add_executable(app ${SOURCES})

target_include_directories(app
    PRIVATE ${GTKMM_INCLUDE_DIRS}
    PUBLIC ${PROJECT_SOURCE_DIR}/inc
)
target_link_libraries(app
    ${GTKMM_LIBRARIES} 
    ${MYSQLCLIENT_LIB}
)
target_compile_options(app PRIVATE ${GTKMM_CFLAGS_OTHER})
