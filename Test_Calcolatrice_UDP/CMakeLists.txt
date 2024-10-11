cmake_minimum_required(VERSION 3.26)
project(Test C)

set(CMAKE_C_STANDARD 11)

# Aggiungi i percorsi dei file sorgente per Client e Server
set(Client_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Client/Client.c
)

set(Server_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Server/Server.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Server/Calculator.c
)

# Crea i target eseguibili per Client e Server
add_executable(Client ${Client_SOURCES})
add_executable(Server ${Server_SOURCES})

# Collega la libreria ws2_32
target_link_libraries(Server PRIVATE ws2_32)
target_link_libraries(Client PRIVATE ws2_32)