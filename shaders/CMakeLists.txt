file(GLOB FRAG "*.frag")
file(GLOB VERT "*.vert")

set(SHADER_SRC ${FRAG} ${VERT})
set(SHADER_OUT "${FRAG}.spv" "${VERT}.spv")

message(${SHADER_OUT})

add_custom_target(shaders SOURCES ${SHADER_SRC} COMMENT "Compile shaders" COMMAND "glslc" ${SHADER_SRC} -o -c ${SHADER_OUT})