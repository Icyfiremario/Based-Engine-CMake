file(GLOB SHADERS CONFIGURE_DEPENDS "*.vs" "*.fs")

set(COPIED_SHADERS)
foreach (SHADER ${SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(COPY_OUTPUT "${CMAKE_BINARY_DIR}/shaders/opengl/${FILE_NAME}")

    add_custom_command(
            OUTPUT ${COPY_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} ${COPY_OUTPUT}
            DEPENDS ${SHADER}
            COMMENT "Copying ${FILE_NAME} to ${COPY_OUTPUT}"
            VERBATIM
    )

    list(APPEND COPIED_SHADERS ${COPY_OUTPUT})
endforeach (SHADER ${SHADERS})

add_custom_target(CopyGLShaders DEPENDS ${COPIED_SHADERS})