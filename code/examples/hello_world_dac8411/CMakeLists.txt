add_executable(hello_world_dac8411
        hello_world_dac8411.c
        )

# pull in common dependencies
target_link_libraries(hello_world_dac8411 pico_stdlib hardware_spi)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_world_dac8411)
# enable usb output, disable uart output
pico_enable_stdio_usb(hello_world_dac8411 1)
pico_enable_stdio_uart(hello_world_dac8411 0)