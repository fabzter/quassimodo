file(GLOB OPCIONES_SRC CONFIGURE_DEPENDS "*.cpp")
add_library(Opciones SHARED ${OPCIONES_SRC})
target_include_directories(Opciones PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Opciones PUBLIC Reglas)

find_package(Boost REQUIRED COMPONENTS program_options filesystem)
target_link_libraries(Opciones PUBLIC Boost::program_options Boost::filesystem)
