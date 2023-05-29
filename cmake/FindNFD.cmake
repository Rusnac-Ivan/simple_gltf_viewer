# FindNFD - Native File Dialog Extended.

# default search dirs    
set(_nfd_HEADER_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include"
    "${CMAKE_SOURCE_DIR}/includes"
    "C:/Program Files (x86)/glm" )

message(STATUS "NFD_ROOT: ${NFD_ROOT}")

if(NFD_ROOT)
	set(_nfd_HEADER_SEARCH_DIRS "${NFD_ROOT}"
        "${NFD_ROOT}"
        ${_nfd_HEADER_SEARCH_DIRS})
endif(NFD_ROOT)

# Search for the include path
find_path(NFD_INCLUDE_DIR "nfd.hpp"
            PATHS ${_nfd_HEADER_SEARCH_DIRS}
            PATH_SUFFIXES include
        )
message(STATUS "NFD_INCLUDE_DIR: ${NFD_INCLUDE_DIR}")

# Search for the libraries
set(NFD_LIBS nfd)
set(NFD_LIBRARY_RELEASE   "")
set(NFD_LIBRARY_DEBUG     "")

foreach(LIB ${NFD_LIBS})
    find_library(FOUND_LIB_${LIB}_RELEASE NAMES ${LIB}
                PATHS ${_nfd_HEADER_SEARCH_DIRS} 
                PATH_SUFFIXES lib
                CMAKE_FIND_ROOT_PATH_BOTH BOTH
                )
                
    find_library(FOUND_LIB_${LIB}_DEBUG NAMES ${LIB}d
                PATHS ${_nfd_HEADER_SEARCH_DIRS}
                PATH_SUFFIXES lib
                CMAKE_FIND_ROOT_PATH_BOTH BOTH
                )

    list(APPEND NFD_LIBRARY_RELEASE ${FOUND_LIB_${LIB}_RELEASE})
    list(APPEND NFD_LIBRARY_DEBUG ${FOUND_LIB_${LIB}_DEBUG})
    message("Lib: ${LIB}")
    message("Found Release Lib: ${FOUND_LIB_${LIB}_RELEASE}")
    message("Found Debug Lib: ${FOUND_LIB_${LIB}_DEBUG}")
endforeach(LIB)

#INCLUDE(FindPackageHandleStandardArgs)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG NFD_LIBRARY NFD_INCLUDE_DIR)

include(SelectLibraryConfigurations)
select_library_configurations(NFD)
message(STATUS "NFD_LIBRARY: ${NFD_LIBRARY}")
message(STATUS "NFD_LIBRARY_RELEASE: ${NFD_LIBRARY_RELEASE}")
message(STATUS "NFD_LIBRARY_DEBUG: ${NFD_LIBRARY_DEBUG}")
