// Current control
//
// Copyright 2017 Todd Fleming
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "current_control.h"
#include "grbl.h"

#ifdef HAS_DIGIPOTS

void current_init()
{
    static const uint8_t digipot_motor_current[] = DIGIPOT_MOTOR_CURRENT;
    // if the SS pin is not already configured as an output
    // then set it high (to enable the internal pull-up resistor)
    // if(!(SPI_PORT & SS_BIT)){
    //     SPI_PORT |= (1 << SS);
    // }

    // Set DIGIPOTSS_PIN as output
    DIGIPOTSS_DDR |= 1<<DIGIPOTSS_BIT;
    // Set MOSI, SCK as Output
    SPI_DDR |= (1<<MOSI_BIT)|(1<<SCK_BIT)

    // Initialize SPI as master with clk/16
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

    for (uint8_t i = 0; i < COUNT(digipot_motor_current); i++) {
        //digitalPotWrite(digipot_ch[i], digipot_motor_current[i]);
        set_current(i, digipot_motor_current[i]);
    }
}

inline static uint8_t spi_transfer(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1<<SPIF))); // wait
    return SPDR;
}

void set_current(uint8_t motor, float amps)
{
    // Take the SS pin low to select the chip
    DIGIPOTSS_PORT &= ~(1<<DIGIPOTSS_BIT)
    // Send the address and value via SPI
    spi_transfer(address);
    spi_transfer(value);
    // Take the SS pin high to de-select the chip
    DIGIPOTSS_PORT |= (1<<DIGIPOTSS_BIT)
}

#endif // HAS_DIGIPOTS
