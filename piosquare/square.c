/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
// Our assembled program:
#include "square.pio.h"

int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning pio/hello_pio example requires a board with a regular LED
#else
    // Choose which PIO instance to use (there are two instances)
    PIO pio = pio0;

    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset = pio_add_program(pio, &piosquarewave_program);

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    uint sm = pio_claim_unused_sm(pio, true);

    // calculate divider for 10MHz pio clock (system runs at 125MHz)
    float div = (float)clock_get_hz(clk_sys) / 10000000.0;
    piosquarewave_program_init(pio, sm, offset, PICO_DEFAULT_LED_PIN, div);

    // The state machine is now running. Any value we push to its TX FIFO will
    // appear on the LED pin.  Pin 26 follows LED pin, Pin 27 is inverted

    uint32_t data = 0;
    while (true)
    {
        // Put data into FIFO
        pio_sm_put_blocking(pio, sm, data);

        // invert value
        data = ~data;

        //sleep
        sleep_ms(1000);
    }
#endif
}
