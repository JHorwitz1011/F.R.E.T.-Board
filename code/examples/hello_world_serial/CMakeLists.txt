add_executable(hello_world_serial
        hello_serial.c
        )

# pull in common dependencies
target_link_libraries(hello_world_serial pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_world_serial)

pico_enable_stdio_usb(hello_world_serial 1)
pico_enable_stdio_uart(hello_world_serial 0)