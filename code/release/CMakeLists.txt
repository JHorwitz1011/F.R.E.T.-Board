add_executable( test
                test.c
                #peripherals
                ad7685.c dac8411.c
                #filtering
                filters.c
                util.c
        )

# pull in common dependencies
target_link_libraries(test pico_stdlib hardware_spi hardware_i2c)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(test)

# enable usb output, disable uart output
pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

add_executable( release
                main.c
                #peripherals
                ad7685.c dac8411.c
                #filtering
                filters.c
                util.c
        )

# pull in common dependencies
target_link_libraries(release pico_stdlib hardware_spi hardware_i2c pico_multicore)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(release)

# enable usb output, disable uart output
pico_enable_stdio_usb(release 1)
pico_enable_stdio_uart(release 0)

