file(GLOB AGENTEWRAP_SRC CONFIGURE_DEPENDS "*.cpp")
# The Python extension MUST be named Reglas.so (agents do `import Reglas`).
add_library(ReglasModule MODULE ${AGENTEWRAP_SRC})
set_target_properties(ReglasModule PROPERTIES OUTPUT_NAME "Reglas" PREFIX "" SUFFIX ".so")
target_include_directories(ReglasModule PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

find_package(Python3 REQUIRED COMPONENTS Development)
find_package(Boost REQUIRED COMPONENTS python)
target_link_libraries(ReglasModule PRIVATE Reglas Boost::python Python3::Module)
if(APPLE)
  target_link_options(ReglasModule PRIVATE -Wl,-undefined,dynamic_lookup)
endif()
# So the module can locate libReglas.dylib at load time:
set_target_properties(ReglasModule PROPERTIES BUILD_RPATH "$<TARGET_FILE_DIR:Reglas>")
