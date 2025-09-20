target_sources (${project_name} PUBLIC
    main.cpp
    bot.cpp
    download.cpp
)

target_include_directories(${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
