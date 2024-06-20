add_library(HEPReference SHARED
    Reference.cc
    Library.cc
)
target_link_libraries(HEPReference PRIVATE project_options project_warnings
                                   PUBLIC fmt::fmt yaml::cpp json::json)

add_library(HEPRequest SHARED
    ArxivRequest.cc
)
target_link_libraries(HEPRequest PRIVATE project_options project_warnings
                                 PUBLIC fmt::fmt yaml::cpp json::json cpr spdlog::spdlog
                                        pugixml)

add_executable(main cpr.cc)
target_link_libraries(main PRIVATE project_options project_warnings
                           PUBLIC fmt yaml::cpp json::json cpr spdlog
                                  range-v3)

add_executable(NuHepMCParse NuHepMCParse.cc NuHepMC.cc)
target_link_libraries(NuHepMCParse PRIVATE project_options project_warnings
                                   PUBLIC fmt cpr spdlog NuHepMC::CPPUtils range-v3)

set_target_properties(NuHepMCParse PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
