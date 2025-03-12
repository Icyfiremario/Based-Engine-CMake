file(GLOB SHADERS "*.vert" "*.frag" "*.comp" "*.geom" "*.tesc" "*.tese")

set(COMPILED_SHADERS)
set(GLSLC_EXECUTABLE "/usr/bin/glslc")

foreach(SHADER ${SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    get_filename_component(DIR ${SHADER} DIRECTORY)
    set(OUTPUT_SHADER "${CMAKE_BINARY_DIR}/shaders/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${OUTPUT_SHADER}
        COMMAND ${GLSLC_EXECUTABLE} ${SHADER} -o ${OUTPUT_SHADER}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${FILE_NAME}"
        VERBATIM
    )
    
    list(APPEND COMPILED_SHADERS ${OUTPUT_SHADER})
endforeach(SHADER ${SHADERS})

add_custom_target(shaders DEPENDS ${COMPILED_SHADERS})
