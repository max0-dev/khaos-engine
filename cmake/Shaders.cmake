# project_add_shader_check(<target> <directory>)
# Creates <target>, which checks every GLSL shader under <directory> with glslangValidator,
# the Khronos reference compiler, so a broken shader fails the build like broken C++ does.
# Shaders are only checked, not compiled: OpenGL still loads the .vert/.frag sources at runtime.
# The stage comes from the extension, so plain .glsl files (no stage) are skipped.
# Each shader is rechecked only when it changes; new shaders are picked up on the next build.
find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)

function(project_add_shader_check target directory)
    file(GLOB_RECURSE shaders
        CONFIGURE_DEPENDS
        ${directory}/*.vert
        ${directory}/*.tesc
        ${directory}/*.tese
        ${directory}/*.geom
        ${directory}/*.frag
        ${directory}/*.comp
    )

    set(stamps)
    foreach(shader IN LISTS shaders)
        file(RELATIVE_PATH name ${CMAKE_SOURCE_DIR} ${shader})
        set(stamp ${CMAKE_CURRENT_BINARY_DIR}/${target}/${name}.stamp)

        # The stamp is touched only when the check passes, so a failed shader is checked again
        add_custom_command(
            OUTPUT ${stamp}
            COMMAND ${GLSLANG_VALIDATOR} ${shader}
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp}
            DEPENDS ${shader} ${GLSLANG_VALIDATOR}
            COMMENT "Checking shader ${name}"
            VERBATIM
        )
        list(APPEND stamps ${stamp})
    endforeach()

    add_custom_target(${target} ALL DEPENDS ${stamps} SOURCES ${shaders})
endfunction()
