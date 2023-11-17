add_executable( main
                main.c
        )

# pull in common dependencies
target_link_libraries(main pico_stdlib hardware_spi)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(main)

# enable usb output, disable uart output
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)