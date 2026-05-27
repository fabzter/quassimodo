file(GLOB REGLAS_SRC CONFIGURE_DEPENDS "*.cpp")
# No source defines main() — all files go into the shared library.
add_library(Reglas SHARED ${REGLAS_SRC})
target_include_directories(Reglas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
find_package(Boost REQUIRED COMPONENTS filesystem system)
target_link_libraries(Reglas PUBLIC Boost::filesystem Boost::system)
