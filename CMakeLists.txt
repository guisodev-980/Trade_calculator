cmake_minimum_required(VERSION 3.10)
project(TraderApp)

set(CMAKE_CXX_STANDARD 17)

# Diretórios de headers
include_directories(include)

# Arquivos fonte
file(GLOB SOURCES "src/*.cpp")

# Criar executável
add_executable(trader ${SOURCES})
