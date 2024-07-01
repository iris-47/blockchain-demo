find_package(SFML 2.5 REQUIRED COMPONENTS graphics audio network)
file(GLOB_RECURSE SOURCES "*.cpp")

link_libraries(sfml-graphics sfml-audio sfml-network ImGui-SFML)
add_subdirectory(utils)

set(LIBRARIES
    application
)

add_executable(BlockchainDemo ${SOURCES})
target_link_libraries(BlockchainDemo ${LIBRARIES})
