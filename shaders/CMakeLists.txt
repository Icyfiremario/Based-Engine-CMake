file(GLOB VK_SHADERS "*.vert" "*.frag" "*.comp" "*.geom" "*.tesc" "*.tese") 
file(GLOB GL_SHADERS "*.vs" "*.fs") # Current code doesn't load pre-compiled shaders for OpenGL
file(COPY ${GL_SHADERS} DESTINATION "${CMAKE_BINARY_DIR}/gl_shaders")

set(COMPILED_VK_SHADERS)

foreach(SHADER ${VK_SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    get_filename_component(DIR ${SHADER} DIRECTORY)
    set(OUTPUT_SHADER "${CMAKE_BINARY_DIR}/shaders/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${OUTPUT_SHADER}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${OUTPUT_SHADER}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${FILE_NAME}"
        VERBATIM
    )
    
    list(APPEND COMPILED_VK_SHADERS ${OUTPUT_SHADER})
endforeach(SHADER ${VK_SHADERS})

add_custom_target(Shaders DEPENDS ${COMPILED_VK_SHADERS})
