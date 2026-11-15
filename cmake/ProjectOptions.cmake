# project_setup_target(<target>)
# Gives a target of ours the shared warnings and, if enabled, the sanitizers.
# Only for our own targets: vcpkg builds the dependencies with its own flags.
function(project_setup_target target)
    target_compile_options(${target} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wnull-dereference
        -Wimplicit-fallthrough
    )

    if(WARNINGS_AS_ERRORS)
        target_compile_options(${target} PRIVATE -Werror)
    endif()

    if(ENABLE_SANITIZERS)
        # -fno-sanitize-recover=all: abort on the first UBSan report (like ASan) instead of
        # printing and continuing, so a test that triggers UB fails.
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined
            -fno-sanitize-recover=all
            -fno-omit-frame-pointer
        )
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()
