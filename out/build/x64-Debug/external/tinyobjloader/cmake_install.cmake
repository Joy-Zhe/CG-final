# Install script for directory: C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/external/tinyobjloader

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/out/build/x64-Debug/external/tinyobjloader/tinyobjloader.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake/tinyobjloader-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake/tinyobjloader-targets.cmake"
         "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/out/build/x64-Debug/external/tinyobjloader/CMakeFiles/Export/ed38dc2cb1e14c44cc057c68af26fb32/tinyobjloader-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake/tinyobjloader-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake/tinyobjloader-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/out/build/x64-Debug/external/tinyobjloader/CMakeFiles/Export/ed38dc2cb1e14c44cc057c68af26fb32/tinyobjloader-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/out/build/x64-Debug/external/tinyobjloader/CMakeFiles/Export/ed38dc2cb1e14c44cc057c68af26fb32/tinyobjloader-targets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/external/tinyobjloader/tiny_obj_loader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/tinyobjloader" TYPE FILE FILES "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/external/tinyobjloader/LICENSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES
    "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/out/build/x64-Debug/external/tinyobjloader/tinyobjloader-config.cmake"
    "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/out/build/x64-Debug/external/tinyobjloader/tinyobjloader-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/Joy Zheng/Documents/WeChat Files/wxid_3xl3x3188tfz21/FileStorage/File/2023-05/final/out/build/x64-Debug/external/tinyobjloader/tinyobjloader.pc")
endif()

