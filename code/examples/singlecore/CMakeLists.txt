add_executable( singlecore
                main.c
                filters.c
                ad7685.c
                util.c
                dac8411.c
                ads7830.c
        )

# pull in common dependencies
target_link_libraries(singlecore pico_stdlib hardware_spi hardware_i2c)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(singlecore)

# enable usb output, disable uart output
pico_enable_stdio_usb(singlecore 1)
pico_enable_stdio_uart(singlecore 0)