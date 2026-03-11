cmake_minimum_required(VERSION 3.11)
project(Sueca_Game CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Garante que o CMake olhe no local correto do Homebrew
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")

# No SFML 3, use nomes com Capitalize: Graphics, Window, System
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Pega todos os fontes na pasta src
file(GLOB project_SRCS src/*.cpp)

add_executable(sueca ${project_SRCS})

# No SFML 3, o nome dos alvos para linkar também mudou para o padrão Namespace
target_link_libraries(sueca SFML::Graphics SFML::Window SFML::System)

if(APPLE)
    set_target_properties(sueca PROPERTIES MACOSX_BUNDLE TRUE)
endif()

# ... (mantenha o restante do arquivo como está)

# Comando para copiar a pasta data para o diretório de build
add_custom_command(TARGET sueca POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    $<TARGET_FILE_DIR:sueca>/data
)

# Se estiver no macOS, precisamos garantir que a pasta data vá para dentro do Bundle também
if(APPLE)
    add_custom_command(TARGET sueca POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        $<TARGET_BUNDLE_CONTENT_DIR:sueca>/Resources/data
    )
endif()