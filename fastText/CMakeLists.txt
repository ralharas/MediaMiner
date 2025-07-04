cmake_minimum_required(VERSION 3.10)
project(fasttext)

# Enable C++11 features
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Add the necessary source files
add_library(fasttext STATIC
    fasttext.cc
    args.cc
    loss.cc
    main.cc
    matrix.cc
    meter.cc
    model.cc
    productquantizer.cc
    quantmatrix.cc
    utils.cc
    vector.cc
    densematrix.cc 
)

# Include the current directory (and subdirectories) for headers
target_include_directories(fasttext PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
