file(GLOB MODELS CONFIGURE_DEPENDS "*.wobj" "*.obj") # *.wobj is just in case CMake thinks that *.obj is a compiled file

set(COPIED_MODELS)
set(MODELS_LOCATION "${CMAKE_BINARY_DIR}/3D_Models")

foreach(MODEL ${MODELS})
    get_filename_component(FILE_NAME ${MODEL} NAME)
    set(COPY_OUTPUT "${MODELS_LOCATION}/${FILE_NAME}")

    add_custom_command(
        OUTPUT ${COPY_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E copy ${MODEL} ${COPY_OUTPUT}
        DEPENDS ${MODEL}
        COMMENT "Copying ${FILE_NAME} to ${COPY_OUTPUT}"
        VERBATIM
    )

    list(APPEND COPIED_MODELS ${COPY_OUTPUT})
endforeach(MODEL ${MODELS})

add_custom_target(CopyModels DEPENDS ${COPIED_MODELS})
