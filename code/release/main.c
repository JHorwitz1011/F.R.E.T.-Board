#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "pico/multicore.h"

#include "dac8411.h"
#include "ad7685.h"
#include "filters.h"
#include "pins.h"

#define FIFO_TIMEOUT_US 10
#define SYS_FS 44100 //hz
#define SYS_PERIOD 1000000/SYS_FS
spi_inst_t* spi;
i2c_inst_t* i2c;

bool timer_callback(repeating_timer_t *rt) {
    gpio_put(UTIL1, 1);
    sleep_us(1);
    gpio_put(UTIL1, 0);
    // //read new value from adc to memory
    // //1 pull from adc
    // //2 push out to queue
    
    // if(!multicore_fifo_push_timeout_us(FIFO_TIMEOUT_US, 1000)) {
    //     // while(1){   
    //         // printf("ERROR POPPING DATA. 1\n");
    //     // }
    //     gpio_put(UTIL2, 1);
    //     sleep_us(1);
    //     gpio_put(UTIL2, 0);  
    // }

    // //take value from memory and push it out to dac
    // //3 pull value from queue/wait to pull
    // sfint data = multicore_fifo_pop_blocking();    
    // //4 push to dac
    // // dac8411_write(spi, DAC8411_POWER_NORMAL, data);
    // printf("%f", data);

    return true; //keep repeating
}

void core1_entry() {
    repeating_timer_t timer;

    // // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_us(-SYS_PERIOD, timer_callback, NULL, &timer)) {
        printf("Failed to add timer\n");
        while(1) {
            gpio_put(UTIL1, 1);
            sleep_us(1);
            gpio_put(UTIL1, 0);  
            sleep_us(1);
        }
    }

    //loop go brr
    while(1){
        tight_loop_contents();
    }
}

int main() {
    Coeff* coeffs;
    Gains* gains;
    
    stdio_init_all();

    gpio_init(UTIL1);
    gpio_set_dir(UTIL1, GPIO_OUT);
    gpio_put(UTIL1, 1);
    gpio_init(UTIL2);
    gpio_set_dir(UTIL2, GPIO_OUT);
    gpio_put(UTIL2, 1);

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

    // multicore_launch_core1(core1_entry);
    // sleep_us(100);
    //start core 1
    // We MUST start the other core before we enabled FIFO interrupts.
    // This is because the launch uses the FIFO's, enabling interrupts before
    // they are used for the launch will result in unexpected behaviour.
    repeating_timer_t timer;

    if (!add_repeating_timer_us(-SYS_PERIOD, timer_callback, NULL, &timer)) {
        printf("Failed to add timer\n");
        while(1) {
            gpio_put(UTIL1, 1);
            sleep_us(1);
            gpio_put(UTIL1, 0);  
            sleep_us(1);
        }
    }
    //loop forever
    while(1) {

        // printf("looping..\n");
        

        // //shift buffers
        // inputs[0] = inputs[1];
        // inputs[1] = inputs[2];

        // outputs[0] = outputs[1];
        // outputs[1] = outputs[2];
 
        // //pop from queue
        // if(!multicore_fifo_pop_timeout_us(FIFO_TIMEOUT_US, (uint32_t*)(inputs+2))) {
        //     gpio_put(UTIL2, 1);
        //     sleep_us(1);
        //     gpio_put(UTIL2, 0);
        // }
        
        // outputs[2] = filter(inputs, outputs, coeffs, gains);
        
        // if(!multicore_fifo_push_timeout_us(outputs[2], FIFO_TIMEOUT_US)) {
        //     gpio_put(UTIL2, 1);
        //     sleep_us(1);
        //     gpio_put(UTIL2, 0);        
        // }
    }
}