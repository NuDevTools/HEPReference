CPMFindPackage(
    NAME fmt
    GIT_TAG 7.1.3
    GITHUB_REPOSITORY fmtlib/fmt
)
add_library(fmt::fmt ALIAS fmt)

CPMFindPackage(
    NAME spdlog
    GIT_TAG "v1.8.5"
    GITHUB_REPOSITORY gabime/spdlog
    OPTIONS
        "SPDLOG_COMPILED_LIB ON"
        "SPDLOG_FMT_EXTERNAL ON"
        "SPDLOG_BUILD_SHARED ON"
)
add_library(spdlog::spdlog ALIAS spdlog)

CPMAddPackage(
    NAME yaml-cpp
    GITHUB_REPOSITORY jbeder/yaml-cpp
    GIT_TAG 4edff1fa5dbfca16fc72d89870841bee89f8ef89
    OPTIONS
        "YAML_CPP_BUILD_TESTS OFF"
        "YAML_CPP_BUILD_CONTRIB OFF"
        "YAML_CPP_BUILD_TOOLS OFF"
)
add_library(yaml::cpp ALIAS yaml-cpp)

CPMAddPackage(
    NAME json
    GITHUB_REPOSITORY nlohmann/json 
    GIT_TAG  v3.10.4
)
add_library(json::json ALIAS nlohmann_json)

CPMAddPackage(
    NAME cpr
    GITHUB_REPOSITORY libcpr/cpr
    GIT_TAG 1.7.2
)

CPMAddPackage(
    NAME pugixml
    GITHUB_REPOSITORY zeux/pugixml
    VERSION 1.11.4
)

CPMAddPackage(
    NAME range-v3
    GITHUB_REPOSITORY ericniebler/range-v3
    GIT_TAG 0.12.0
    OPTIONS
        "RANGE_V3_DOCS OFF"
        "RANGE_V3_TESTS OFF"
        "RANGE_V3_EXAMPLES OFF"
        "RANGE_V3_PERF OFF"
        "RANGE_V3_HEADER_CHECKS OFF"
)

# Install testing framework
if(ENABLE_TESTING OR ENABLE_FUZZING)
    # Catch2
    CPMAddPackage(
        NAME catch2
        GITHUB_REPOSITORY catchorg/Catch2
        GIT_TAG cfb695669830ceb73b0875a20af4241944eb48e0
    )
    add_library(Catch2::Catch2 ALIAS Catch2)

    # Trompeloeil
    CPMAddPackage(
        NAME trompeloeil
        GITHUB_REPOSITORY rollbear/trompeloeil
        VERSION 37 
    )
endif()
