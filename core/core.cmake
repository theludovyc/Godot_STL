generateHeader(core/authors)

generateHeader(core/io/certs_compressed)

generateSource(core/input/default_controller_mappings)

generateHeader(core/donors)

generateHeader(core/license)

generateSource(core/script_encryption_key)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/core/version_generated.gen.h ${CMAKE_CURRENT_SOURCE_DIR}/core/version_hash.gen.h
        COMMAND ${Python3_EXECUTABLE} pygen_script/generate_version.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS pygen_script/generate_version.py
)

list(APPEND SRC ${CMAKE_CURRENT_SOURCE_DIR}/core/version_generated.gen.h ${CMAKE_CURRENT_SOURCE_DIR}/core/version_hash.gen.h)

include(thirdparty/mbedtls/mbedtls.cmake)

set(src_core_thirdparty_misc
        thirdparty/misc/fastlz.c
        thirdparty/misc/r128.c
        thirdparty/misc/smaz.c
        thirdparty/misc/pcg.cpp
        thirdparty/misc/polypartition.cpp
        thirdparty/misc/clipper.cpp
        thirdparty/misc/smolv.cpp
        )

include(thirdparty/minizip/minizip.cmake)

include(thirdparty/zlib/zlib.cmake)

include(thirdparty/zstd/zstd.cmake)

file(GLOB_RECURSE src_core core/*.cpp)

list(APPEND SRC ${src_core_thirdparty_misc} ${src_core})
