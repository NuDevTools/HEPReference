add_library(HEPReference SHARED
    Reference.cc
)
target_link_libraries(HEPReference PRIVATE project_options project_warnings
                                   PUBLIC fmt::fmt yaml::cpp json::json)
