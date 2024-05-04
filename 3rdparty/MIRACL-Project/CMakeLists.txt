cmake_minimum_required(VERSION 3.21)
project(Miracl LANGUAGES CXX C)

include(GNUInstallDirs)

set(SOURCE_FILES miracl-source/source/mrcore.c miracl-source/source/mrarth0.c miracl-source/source/mrarth1.c
    miracl-source/source/mrarth2.c miracl-source/source/mralloc.c miracl-source/source/mrsmall.c miracl-source/source/mrio1.c
    miracl-source/source/mrio2.c miracl-source/source/mrgcd.c miracl-source/source/mrjack.c miracl-source/source/mrxgcd.c
    miracl-source/source/mrarth3.c miracl-source/source/mrbits.c miracl-source/source/mrrand.c miracl-source/source/mrprime.c
    miracl-source/source/mrcrt.c miracl-source/source/mrscrt.c miracl-source/source/mrmonty.c miracl-source/source/mrpower.c
    miracl-source/source/mrsroot.c miracl-source/source/mrcurve.c miracl-source/source/mrfast.c miracl-source/source/mrshs.c
    miracl-source/source/mrshs256.c miracl-source/source/mrshs512.c miracl-source/source/mrsha3.c miracl-source/source/mrfpe.c
    miracl-source/source/mraes.c miracl-source/source/mrgcm.c miracl-source/source/mrlucas.c miracl-source/source/mrzzn2.c
    miracl-source/source/mrzzn2b.c miracl-source/source/mrzzn3.c miracl-source/source/mrzzn4.c miracl-source/source/mrecn2.c
    miracl-source/source/mrstrong.c miracl-source/source/mrbrick.c miracl-source/source/mrebrick.c miracl-source/source/mrec2m.c
    miracl-source/source/mrgf2m.c miracl-source/source/mrflash.c miracl-source/source/mrfrnd.c miracl-source/source/mrdouble.c
    miracl-source/source/mrround.c miracl-source/source/mrbuild.c miracl-source/source/mrflsh1.c miracl-source/source/mrpi.c
    miracl-source/source/mrflsh2.c miracl-source/source/mrflsh3.c miracl-source/source/mrflsh4.c)

if(CMAKE_SIZEOF_VOID_P EQUAL 8) # X64 or aarch64
    message(STATUS "MIRACL: 64-bit build")
    if(CMAKE_CXX_COMPILER_ID)
        set(MIRDEF_SUFFIX "hpp")
        list(APPEND SOURCE_FILES miracl-source/source/big.cpp miracl-source/source/zzn.cpp
            miracl-source/source/ecn.cpp miracl-source/source/ec2.cpp miracl-source/source/flash.cpp
            miracl-source/source/crt.cpp)
    else()
        set(MIRDEF_SUFFIX "h64")
    endif()
    if(MSVC)
        set(MIRDEF_SUFFIX "w64")
        set(MRMULDV_SUFFIX "w64")
        set(DIR_FLAG "win64")
    elseif(MINGW)
        set(MIRDEF_SUFFIX "mgwn")
        set(DIR_FLAG "win64")
        set(MRMULDV_SUFFIX "g64")
    else()
        if(APPLE)
            set(DIR_FLAG "macOS")
            set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "" FORCE)
        elseif(WIN32)
            set(DIR_FLAG "win64")
        else()
            set(MIRDEF_SUFFIX "linux64")
            set(DIR_FLAG "linux64")
        endif()
        set(MRMULDV_SUFFIX "g64")
    endif()
else()
    message(STATUS "MIRACL: 32-bit build")
    set(MIRDEF_SUFFIX "h32")
    if(MSVC)
        set(MRMULDV_SUFFIX "c")
        set(DIR_FLAG "win32")
    else()
        if(APPLE)
            set(DIR_FLAG "OSX")
            set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "" FORCE)
        elseif(WIN32)
            set(DIR_FLAG "win32")
        else()
            set(MIRDEF_SUFFIX "linux32")
            set(DIR_FLAG "linux32")
        endif()
        set(MRMULDV_SUFFIX "gcc")
    endif()
endif()


file(GLOB HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/miracl-source/include ${CMAKE_CURRENT_SOURCE_DIR}/miracl-source/include/*.h)
foreach(HEADER_FILE ${HEADER_FILES})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/miracl-source/include/${HEADER_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include)
    list(APPEND SOURCE_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include/${HEADER_FILE})
endforeach()
if(MINGW OR LINUX)
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/fixed_headers/mirdef.${MIRDEF_SUFFIX}"
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include)
else()
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/miracl-source/include/mirdef.${MIRDEF_SUFFIX}"
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include)
endif()
file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include/mirdef.${MIRDEF_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include/mirdef.h)
list(APPEND SOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include/mirdef.h")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/miracl-source/source/mrmuldv.${MRMULDV_SUFFIX}"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/source)
file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/source/mrmuldv.${MRMULDV_SUFFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/source/mrmuldv.c)
list(APPEND SOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/source/mrmuldv.c")

# 生成静态库
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include
)

if(MIRACL_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR ${MIRACL_INSTALL_BINDIR})
endif()

if(MIRACL_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR ${MIRACL_INSTALL_LIBDIR})
endif()

if(MIRACL_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR ${MIRACL_INSTALL_INCLUDEDIR})
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

file(GLOB HEADER_FILES ${CMAKE_CURRENT_BINARY_DIR}/${DIR_FLAG}/include/*.h)
install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
