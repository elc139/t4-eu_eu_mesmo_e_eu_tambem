cmake_minimum_required(VERSION 3.15)
project(Trabalho4 CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        virusim/virusim.cpp
        virusim/Random.cpp
        virusim/Population.cpp)

set(executables virusim virusim_parallel_probs virusim_parallel_trials virusim_parallel_probs_n_trials)

foreach(EXECUTABLE IN LISTS executables)
    add_executable(${EXECUTABLE} ${SOURCE_FILES})
endforeach()

#target_compile_definitions(virusim   PUBLIC ORIGINAL)
target_compile_definitions(virusim_parallel_probs PUBLIC PARALLEL_PROBS)
target_compile_definitions(virusim_parallel_trials PUBLIC PARALLEL_TRIALS)
target_compile_definitions(virusim_parallel_probs_n_trials PUBLIC PARALLEL_PROBS PARALLEL_TRIALS)

if (WIN32)
    foreach(EXECUTABLE IN LISTS executables)
    set_target_properties(${EXECUTABLE} PROPERTIES LINK_FLAGS " --static")
    add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/libs"
            $<TARGET_FILE_DIR:${EXECUTABLE}>)
    endforeach()
endif()

find_package(OpenMP REQUIRED)

foreach(EXECUTABLE IN LISTS executables)
    target_link_libraries(${EXECUTABLE} PUBLIC OpenMP::OpenMP_CXX)
endforeach()