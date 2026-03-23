cmake_minimum_required(VERSION 3.16)
project(UniversalDbManager VERSION 1.0 LANGUAGES CXX)

# Wymagamy standardu C++17 (dla std::unique_ptr i nowszych funkcji)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Bardzo ważne dla Qt: włączamy automatyczne generowanie kodu (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Szukamy instalacji Qt (Wspiera zarówno Qt5 jak i Qt6)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Wskazujemy nasze pliki źródłowe
set(SOURCES
        src/main.cpp
        src/MainWindow.cpp
        src/SQLiteBackend.cpp
)

# Wskazujemy pliki nagłówkowe (przydatne, aby środowisko np. CLion/QtCreator je widziało)
set(HEADERS
        include/IDatabaseBackend.h
        include/SQLiteBackend.h
        include/MainWindow.h
)

# Tworzymy aplikację wykonywalną
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Mówimy kompilatorowi, gdzie szukać plików .h
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Linkujemy nasz program z bibliotekami Qt (GUI oraz SQL)
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
)