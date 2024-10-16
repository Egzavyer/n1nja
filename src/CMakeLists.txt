set(SOURCES
    main.cpp
    peer.cpp
    message.cpp
    messageParser.cpp
)

add_executable(n1nja ${SOURCES})

target_include_directories(n1nja PRIVATE ${AVAHI_INCLUDE_DIRS})

install(TARGETS n1nja DESTINATION bin)
