# Feat
# copyright 2017 William La Cava 
# license: GNU/GPL v3

######### Options ###########################
option( CORE_USE_CUDA   "Use CUDA to speed up certain parts of the code."  OFF )
#############################################

######### CUDA decisions ####################
#if (CORE_USE_CUDA)
#    MESSAGE( STATUS ">> --------------  USING CUDA --------------" )
#    set( CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda-8.0")
#    if (APPLE OR UNIX)
#        set(CMAKE_C_COMPILER   /usr/bin/gcc)
#        set(CMAKE_CXX_COMPILER /usr/bin/g++)
#    elseif (WIN32)
#        ### By default we use VS
#        MESSAGE( STATUS ">> User compiler:             MSVC" )
#        MESSAGE( STATUS ">> Choosing a different compiler is not yet implemented for Windows" )
#    endif()
#endif()
#############################################

# version requirement for Eigen
cmake_minimum_required (VERSION 3.0)
# use c++ 14
set (CMAKE_CXX_STANDARD 14)

#set (CMAKE_CXX_FLAGS "--std=c++11")

#set (CUDA_PROPAGATE_HOST_FLAGS ON)

  
# project name
project(Brush)

# add Eigen include paths
option(EIGEN_DIR "use Eigen environment variable" OFF)
if (EIGEN_DIR)
    include_directories("$ENV{EIGEN3_INCLUDE_DIR}")
else()
    find_package(Eigen3 REQUIRED NO_MODULE)
    include_directories(${EIGEN3_INCLUDE_DIR})
endif()

# autocomplete for YouCompleteMe needs commands to be exported
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set a default build type if none was specified
set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' since none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# add OpenMP support
option(OMP "use OpenMP" ON)
if (OMP)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
endif()

######### CUDA decisions ####################
if (CORE_USE_CUDA)
    find_package(CUDA REQUIRED)
    add_definitions(-DUSE_CUDA)
    #enable_language(CUDA)
    list(APPEND CUDA_NVCC_FLAGS "-std=c++11 --expt-relaxed-constexpr --default-stream per-thread -Wno-deprecated-gpu-targets")

endif()
#############################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-attributes")

# gather list of source files
if (CORE_USE_CUDA)   
    file(GLOB_RECURSE cuda_files "*.cu")
endif()    

file(GLOB_RECURSE sources "*.cc")

# executable

if (CORE_USE_CUDA)
    cuda_add_library(brush_lib SHARED ${sources} ${cuda_files})
else()
    add_library(brush_lib SHARED ${sources})
endif()

