# require version 3.10 or higher
cmake_minimum_required(VERSION 3.10)

#
# Project
#
#   - dictates the output executable filename
#   - Set the project name here
#   - eg. project(MyProjectName)
#
project(OpenGL - OpenWindow)

#
# HAS_TERMINAL
#
#   - on (default): display a terminal window along side the application window
#   - off: prevent terminal window from being displayed
#
option(HAS_TERMINAL "Show a terminal window for STDOUT/STDERR" on)

#
# C_CXX_SOURCES_DIR
#
#   - the place where your C/C++ source files are located
#
set(C_CXX_SOURCES_DIR "src")

#
# C_CXX_INCLUDES_DIR
#
#   - the place where external, 3rd party includes live, such as lib files
#
# set(C_CXX_INCLUDES_DIR "libs")

#
# C_CXX_VENDORS_DIR
#
#   - the place where external, 3rd party includes live, such as .h files
#   - Uncomment the following lines
#   - Update the C_CXX_VENDOR_DIRS variable to set the include path for specific vendor files
#
# set(C_CXX_VENDORS_DIR "vendor")
# set(C_CXX_VENDORS_DIRS "vendor1" "vendor2")

#
# C_CXX_HEADERS_DIR
#
#   - the place where your C/C++ header files are located
#
# set(C_CXX_HEADERS_DIR "include")

#
# C_CXX_DATA_DIR
#
#   - the place where your data assets live
#
# set(C_CXX_DATA_DIR "data")

##########################################################################
# DO NOT EDIT BELOW THIS LINE UNLESS YOU KNOW WHAT YOU ARE DOING!!       #
##########################################################################

if(HAS_TERMINAL)
    message(STATUS "Terminal: on")
else()
    message(STATUS "Terminal: off")
endif()

# detect emscripten environment
if (${CMAKE_CXX_COMPILER} MATCHES ".*em[+][+]")
    set(EMSCRIPTEN TRUE)
else()
    set(EMSCRIPTEN FALSE)
endif()

# Set C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# output executable basename
set(OutputExecutable "${CMAKE_PROJECT_NAME}")

# output html for emscripten
if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif (EMSCRIPTEN)

# Directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Source Files are Curated Here
file(
    GLOB_RECURSE SourceFiles
    "${C_CXX_SOURCES_DIR}/*.c"
    "${C_CXX_SOURCES_DIR}/*.cpp"
)

# Executable aka binary output
add_executable(${OutputExecutable} ${SourceFiles})

# OpenGL
if (NOT EMSCRIPTEN)
    set(OpenGL_GL_PREFERENCE LEGACY)
    find_package(OpenGL REQUIRED)
    include_directories(${OpenGL_INCLUDE_DIRS})
    target_link_libraries(${OutputExecutable} ${OpenGL_LIBRARIES} OpenGL::GL)
endif (NOT EMSCRIPTEN)

# Apple Specifics
if (APPLE AND NOT EMSCRIPTEN)

    # Carbon
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
    target_link_libraries(${OutputExecutable} ${CARBON_LIBRARY})

    # GLUT
    find_package(GLUT REQUIRED)
    target_link_libraries(${OutputExecutable} ${GLUT_LIBRARIES})

endif (APPLE AND NOT EMSCRIPTEN)
    
# Linux Specifics
if (UNIX AND NOT APPLE AND NOT EMSCRIPTEN)

    # X11
    find_package(X11 REQUIRED)
    target_link_libraries(${OutputExecutable} ${X11_LIBRARIES})

    include_directories(${X11_INCLUDE_DIRS})

endif (UNIX AND NOT APPLE AND NOT EMSCRIPTEN)

# UNIX or UNIX-like (Linux, MacOS, MSYS2, Cygwin, etc)
if (UNIX)
    
    # Threads
    if (NOT EMSCRIPTEN)
        find_package(Threads REQUIRED)
        target_link_libraries(${OutputExecutable} ${Threads_LIBRARIES})
        
        include_directories(${Threads_INCLUDE_DIRS})
    endif (NOT EMSCRIPTEN)
    
    # PNG
    if (EMSCRIPTEN)
        execute_process(COMMAND embuilder build libpng OUTPUT_QUIET)
        execute_process(COMMAND embuilder build zlib OUTPUT_QUIET)
    endif (EMSCRIPTEN)
    
    find_package(PNG REQUIRED)
    target_link_libraries(${OutputExecutable} ${PNG_LIBRARIES})
    include_directories(${PNG_INCLUDE_DIRS})

    if (NOT APPLE AND NOT MINGW AND NOT CYGWIN)
        find_package(OpenAL REQUIRED)
        target_link_libraries(${OutputExecutable} ${OPENAL_LIBRARY})
        include_directories(${OPENAL_INCLUDE_DIRS})
        target_compile_definitions(${OutputExecutable} PUBLIC USE_OPENAL)
    endif (NOT APPLE AND NOT MINGW AND NOT CYGWIN)
    
    set_target_properties(${OutputExecutable} PROPERTIES COMPILE_FLAGS "-ggdb3 -Og")
    
endif (UNIX)

# Windows Specifics
if (WIN32 AND NOT EMSCRIPTEN)
    
    # We need to specify the output for each configuration to make it work
    # on Visual Studio solutions.
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_PROFILE "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_PROFILE "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_PROFILE "${CMAKE_BINARY_DIR}/bin")

    if (MSVC)
        # set the startup project to the target executable instead of ALL_BUILD
        set_property(
            DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
            PROPERTY
            VS_STARTUP_PROJECT
            ${OutputExecutable}
        )
        
        # set working directory for Visual Studio Debugger
        set_target_properties(
            ${OutputExecutable} PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
        
        # set subsytem, console if HAS_TERMINAL is true. windows if not
        if (HAS_TERMINAL)
            target_link_options(${OutputExecutable} PRIVATE "/SUBSYSTEM:CONSOLE")
        else ()
            target_link_options(${OutputExecutable} PRIVATE "/SUBSYSTEM:WINDOWS")
        endif ()

    endif (MSVC)
    
    # prevent console launch when compiled with MinGW
    if (MINGW)
        if (NOT HAS_TERMINAL)
            target_link_libraries(${OutputExecutable} -mwindows)
        endif (NOT HAS_TERMINAL)
    endif (MINGW)

    # GDI+
    set(GDIPLUS_LIBRARY gdiplus)
    target_link_libraries(${OutputExecutable} ${GDIPLUS_LIBRARY})
    
    # Shlwapi
    set(SHLWAPI_LIBRARY shlwapi)
    target_link_libraries(${OutputExecutable} ${SHLWAPI_LIBRARY})
    
    # Dwmapi
    set(DWMAPI_LIBRARY dwmapi)
    target_link_libraries(${OutputExecutable} ${DWMAPI_LIBRARY})

    # winmm
    set(WINMM_LIBRARY winmm)
    target_link_libraries(${OutputExecutable} ${WINMM_LIBRARY})
    
endif (WIN32 AND NOT EMSCRIPTEN)

# Emscripten specifics
if (EMSCRIPTEN)
    
    set(ASSETS "${CMAKE_SOURCE_DIR}/data")
    if(EXISTS "${ASSETS}" AND IS_DIRECTORY "${ASSETS}")
        target_link_options(
            ${OutputExecutable}
            PRIVATE
            -sALLOW_MEMORY_GROWTH=1
            -sMAX_WEBGL_VERSION=2
            -sMIN_WEBGL_VERSION=2
            -sUSE_LIBPNG=1
            --preload-file ${ASSETS}@data)
    else()
        target_link_options(
            ${OutputExecutable}
            PRIVATE
            -sALLOW_MEMORY_GROWTH=1
            -sMAX_WEBGL_VERSION=2
            -sMIN_WEBGL_VERSION=2
            -sUSE_LIBPNG=1)
    endif()

endif (EMSCRIPTEN)


# Not on Mac; Not with MSVC; Not with Emscripten
if (NOT APPLE AND NOT MSVC AND NOT EMSCRIPTEN)
    
    # C++ Filesystem
    target_link_libraries(${OutputExecutable} stdc++fs)

endif (NOT APPLE AND NOT MSVC AND NOT EMSCRIPTEN)

# set include directory
if (DEFINE C_CXX_HEADERS_DIR AND EXISTS "${CMAKE_SOURCE_DIR}/${C_CXX_HEADERS_DIR}" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${C_CXX_HEADERS_DIR}")
    include_directories("${CMAKE_SOURCE_DIR}/${C_CXX_HEADERS_DIR}")
endif (EXISTS "${CMAKE_SOURCE_DIR}/${C_CXX_HEADERS_DIR}" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${C_CXX_HEADERS_DIR}")

# set lib includes directory
if (DEFINED C_CXX_INCLUDES_DIR AND EXISTS "${C_CXX_INCLUDES_DIR}" AND IS_DIRECTORY "${C_CXX_INCLUDES_DIR}")
    include_directories("${C_CXX_INCLUDES_DIR}")
endif (DEFINED C_CXX_INCLUDES_DIR AND EXISTS "${C_CXX_INCLUDES_DIR}" AND IS_DIRECTORY "${C_CXX_INCLUDES_DIR}")

# set vendor include directors eg. /vendor/lib1, /vendor/lib2...
if (DEFINED C_CXX_VENDORS_DIR AND EXISTS "${C_CXX_VENDORS_DIR}" AND IS_DIRECTORY "${C_CXX_VENDORS_DIR}")
    if (NOT DEFINED C_CXX_VENDORS_DIRS)
        include_directories("${C_CXX_VENDORS_DIR}")
    else
        if (DEFINED C_CXX_VENDORS_DIRS)
            foreach(VN ${C_CXX_VENDORS_DIRS})
                IF(EXISTS "${C_CXX_VENDORS_DIR}/${VN}" AND IS_DIRECTORY "${C_CXX_VENDORS_DIR}/${VN}")
                    include_directories("${C_CXX_VENDORS_DIR}/${VN}")
                endif (EXISTS "${C_CXX_VENDORS_DIR}/${VN}" AND IS_DIRECTORY "${C_CXX_VENDORS_DIR}/${VN}")
            endforeach (VN ${C_CXX_VENDORS_DIRS})
        endif (DEFINED C_CXX_VENDORS_DIRS)
    endif (NOT DEFINED C_CXX_VENDORS_DIRS)
endif (DEFINED C_CXX_VENDORS_DIR AND EXISTS "${C_CXX_VENDORS_DIR}" AND IS_DIRECTORY "${C_CXX_VENDORS_DIR}")


# copy assets folder into build folder exists and is not emscripten
if (DEFINED C_CXX_DATA_DIR AND NOT EMSCRIPTEN AND EXISTS "${CMAKE_SOURCE_DIR}/${C_CXX_DATA_DIR}" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${C_CXX_DATA_DIR}")
    file(COPY "${CMAKE_SOURCE_DIR}/${C_CXX_DATA_DIR}" DESTINATION "${PROJECT_BINARY_DIR}/bin")
endif (DEFINED C_CXX_DATA_DIR AND NOT EMSCRIPTEN AND EXISTS "${CMAKE_SOURCE_DIR}/${C_CXX_DATA_DIR}" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/${C_CXX_DATA_DIR}")