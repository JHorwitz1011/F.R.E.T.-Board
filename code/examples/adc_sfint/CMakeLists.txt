add_executable( adc_sfint
                main.c
                filters.c
                ad7685.c
                util.c
        )

# pull in common dependencies
target_link_libraries(adc_sfint pico_stdlib hardware_spi)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(adc_sfint)

# enable usb output, disable uart output
pico_enable_stdio_usb(adc_sfint 1)
pico_enable_stdio_uart(adc_sfint 0)