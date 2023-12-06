add_executable( sliders
                main.c
                filters.c
                ad7685.c
                util.c
                dac8411.c
                ads7830.c
        )

# pull in common dependencies
target_link_libraries(sliders pico_stdlib hardware_spi hardware_i2c)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(sliders)

# enable usb output, disable uart output
pico_enable_stdio_usb(sliders 1)
pico_enable_stdio_uart(sliders 0)