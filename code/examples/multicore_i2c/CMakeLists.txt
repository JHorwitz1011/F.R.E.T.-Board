add_executable( multicore_i2c
                main.c
                filters.c
                ad7685.c
                util.c
                dac8411.c
                ads7830.c
        )

# pull in common dependencies
target_link_libraries(multicore_i2c pico_stdlib hardware_spi hardware_i2c pico_multicore)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(multicore_i2c)

# enable usb output, disable uart output
pico_enable_stdio_usb(multicore_i2c 1)
pico_enable_stdio_uart(multicore_i2c 0)