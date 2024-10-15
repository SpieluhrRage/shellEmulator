include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
target_link_libraries(shellEmulator ${CONAN_LIBS})
