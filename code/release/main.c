#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/util/queue.h"

#include "dac8411.h"
#include "ad7685.h"
#include "filters.h"

#define FIFO_TIMEOUT_US 1
#define SYS_FS 441000 //hz

spi_inst_t* spi;
i2c_inst_t* i2c;

void core1_entry() {
    repeating_timer_t timer;

    // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_us(-1000000 / SYS_FS, timer_callback, NULL, &timer)) {
        printf("Failed to add timer\n");
        return 1;
    }

    //loop go brr
    while(1)
        tight_loop_contents();
}

bool timer_callback(repeating_timer_t *rt) {
    //read new value from adc to memory
    //1 pull from adc
    //2 push out to queue
    multicore_fifo_push_timeout_us(FIFO_TIMEOUT_US, ad7685_read(spi));

    //take value from memory and push it out to dac
    //3 pull value from queue/wait to pull
    sfint data = multicore_fifo_pop_blocking();    
    //4 push to dac
    dac8411_write(spi, DAC8411_POWER_NORMAL, data);

    return true; //keep repeating
}

int main() {
    Coeff* coeffs;
    Gains* gains;
    
    stdio_init_all();

    //initialize queues
    queue_init(&mem_to_dac_fifo, sizeof(sfint), FIFO_LEN);
    queue_init(&adc_to_mem_fifo, sizeof(sfint), FIFO_LEN);
    
    //initialize filter parameters
    coeffs = initCoefficients();
    gains = initGains();

    //initialize peripherals
    spi = spi0;
    spi_init(spi, 10 * 1000 * 1000); // 10 MHz
    
    i2c = i2c0;
    i2c_init(i2c, 400 * 1000); // 400khz

    //adc/dac
    ad7685_init(spi);
    dac8411_init(spi);

    //initialize input/output arrays
    sfint inputs[3];
    sfint outputs[3];

    //start core 1
    // We MUST start the other core before we enabled FIFO interrupts.
    // This is because the launch uses the FIFO's, enabling interrupts before
    // they are used for the launch will result in unexpected behaviour.
    multicore_launch_core1(core1_entry);

    //loop forever
    while(1) {
        //shift/increment 
        inputs[2] = inputs[1];
        inputs[1] = inputs[0];

        //pop from queue
        if(multicore_fifo_pop_timeout_us(FIFO_TIMEOUT_US, inputs)) {
            while(1)
                printf("ERROR POPPING DATA. 0");
        }
        
        filter(inputs, outputs, coeffs, gains);
        
        multicore_fifo_push_timeout_us(FIFO_TIMEOUT_US, outputs) {
            while(1)
                printf("ERROR PUSHING DATA. 0");
        }
    }
}