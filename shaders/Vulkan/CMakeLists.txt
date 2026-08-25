file(GLOB SHADERS CONFIGURE_DEPENDS "*.vert" "*.frag")

set(COMPILED_SHADERS)
foreach(SHADER ${SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    get_filename_component(DIR ${SHADER} DIRECTORY)
    set(OUTPUT_SHADER "${CMAKE_BINARY_DIR}/shaders/vulkan/${FILE_NAME}.spv")

    add_custom_command(
            OUTPUT ${OUTPUT_SHADER}
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${OUTPUT_SHADER}
            DEPENDS ${SHADER}
            COMMENT "Compiling Vulkan shader: ${FILE_NAME}"
            VERBATIM
    )

    list(APPEND COMPILED_SHADERS ${OUTPUT_SHADER})
endforeach (SHADER ${SHADERS})

add_custom_target(CompileVulkanShaders DEPENDS ${COMPILED_SHADERS})