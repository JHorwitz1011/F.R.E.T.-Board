add_executable( test
                test.c filters.c
        )

# pull in common dependencies
target_link_libraries(test pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(test)

# enable usb output, disable uart output
pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

