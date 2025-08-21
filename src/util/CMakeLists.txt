add_library(afazer_util
    arquivo_lock.cpp
    tempo.cpp
)
target_include_directories(afazer_util PUBLIC ${CMAKE_SOURCE_DIR}/include)