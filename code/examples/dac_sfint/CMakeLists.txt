add_executable( dac_sfint
                main.c
                filters.c
                ad7685.c
                util.c
                dac8411.c
        )

# pull in common dependencies
target_link_libraries(dac_sfint pico_stdlib hardware_spi)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(dac_sfint)

# enable usb output, disable uart output
pico_enable_stdio_usb(dac_sfint 1)
pico_enable_stdio_uart(dac_sfint 0)