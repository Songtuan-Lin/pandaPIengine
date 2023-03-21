project(translator)

# build library for the HTN components
# commands for grapping the source files
file(GLOB HTN_SOURCE_FILES "htn/src/*.cpp") 

add_library(htn INTERFACE)

target_include_directories(htn
    INTERFACE
        htn/include
)

# build the util library
file(GLOB UTIL_SOURCE_FILES 
    "util/src/*.cpp"
    "../Model.cpp" 
    "../ProgressionNetwork.cpp" 
    "../Util.cpp" 
    "../Debug.cpp" 
    "../Invariants.cpp"
)

add_library(tool
    ${UTIL_SOURCE_FILES}
)

target_include_directories(tool   
    PUBLIC
        util/include
        ${pandaPIengine_SOURCE_DIR}
)

# build library for translating to HTN problems
file(GLOB TO_HTN_SOURCE_FILES 
    "to_htn/src/*.cpp"
    "../Model.cpp" 
    "../ProgressionNetwork.cpp" 
    "../Util.cpp" 
    "../Debug.cpp" 
    "../Invariants.cpp"
)

add_library(to_htn
    ${TO_HTN_SOURCE_FILES}
)

target_include_directories(to_htn
    PUBLIC
        to_htn/include
        include
)

target_link_libraries(to_htn
    PUBLIC
        intDataStructures
        htn
        tool
)

file(GLOB FUZZER_SOURCE_FILES
    "fuzzer/src/*.cpp"
)

add_library(fuzzer
    ${FUZZER_SOURCE_FILES}
)

target_include_directories(fuzzer
    PUBLIC
        fuzzer/include
        include
)

target_link_libraries(fuzzer
    PUBLIC
        htn
        tool
)

add_executable(translator
    main.cpp
)

target_include_directories(translator
    PUBLIC
        include
)

target_link_libraries(translator
    PUBLIC
        to_htn
)

add_subdirectory(test)