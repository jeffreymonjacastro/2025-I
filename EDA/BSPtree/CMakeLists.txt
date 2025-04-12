cmake_minimum_required(VERSION 3.10)
project(BSPTreeProject LANGUAGES CXX)

# Como usar:
# mkdir build
# cd build
# cmake ..
# cmake --build . --target run

# Limpiar:
# cmake --build . --target clean-all

# Configuración del estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Archivos fuente
set(SOURCES
    main.cpp
)

# Crear ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Incluir directorio actual para los headers
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Opciones de compilación por sistema
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# -----------------------------
# Target personalizado: run
# -----------------------------
# Detectar ejecutable correcto por sistema
if(WIN32)
    set(EXECUTABLE_NAME "${PROJECT_NAME}.exe")
else()
    set(EXECUTABLE_NAME "${PROJECT_NAME}")
endif()

add_custom_target(run
    COMMAND ${CMAKE_COMMAND} --build . --target ${PROJECT_NAME}
    COMMAND ${EXECUTABLE_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Compilando y ejecutando el proyecto..."
)

# -----------------------------
# Target personalizado: clean-all
# -----------------------------
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Eliminando todos los archivos generados."
)
