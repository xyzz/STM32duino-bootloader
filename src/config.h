/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *****************************************************************************/

/**
 *  @file config.h
 *
 *  @brief bootloader settings and macro defines
 *
 *
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "common.h"



/* Speed controls for strobing the LED pin */
#define BLINK_FAST 0x50000
#define BLINK_SLOW 0x100000


// The USB clock is the same for all boards
#define RCC_APB1ENR_USB_CLK   0x00800000

// Clocks for the backup domain registers
#define RCC_APB1ENR_PWR_CLK   0x10000000
#define RCC_APB1ENR_BKP_CLK   0x08000000

#define USER_CODE    ((u32)0x08002000)

#define LARGEST_FLASH_PAGE_SIZE 0x800

/* Porting information Please read.

    These defineds are use to setup the hardware of the GPIO.
    See http://www.st.com/web/en/resource/technical/document/reference_manual/CD00171190.pdf

    Two GPIO pins need to be defined, the LED and the Button.

    For each pin, the following is required

    LED_BANK, this is the GPIO port, e.g. GPIOA,GPIOB, GPIOC etc etc etc
    LED_PIN, this is the pin number e.g PB1 = 1
    LED_ON_STATE is whether the pin needs to be 1 or 0 for the LED to be lit, this is needed because on some boards the led is wired between Vcc and the Pin
    instead of from the pin to GND

    BUTTON_BANK   Port name of the pin used for the button
    #define BUTTON_PIN
    #define BUTTON_PRESSED_STATE 1
*/

#if defined(TARGET_GENERIC)
    /* Generic stm32f103 target, nothing to see here */
#elif defined(TARGET_BASICPAD_V1)
    /* Middle key in the 3x3 keymatrix */
    #define BL_OUTPUT_BANK GPIOB
    #define BL_OUTPUT_PIN 10
    #define BL_INPUT_BANK GPIOA
    #define BL_INPUT_PIN 4
#elif defined(TARGET_BASICPAD_V2)
    /* Middle key in the 3x3 keymatrix */
    #define BL_OUTPUT_BANK GPIOB
    #define BL_OUTPUT_PIN 12
    #define BL_INPUT_BANK GPIOA
    #define BL_INPUT_PIN 3
#elif defined(TARGET_BASICPOLD)
    /* Backspace */
    #define BL_OUTPUT_BANK GPIOA
    #define BL_OUTPUT_PIN 9
    #define BL_INPUT_BANK GPIOB
    #define BL_INPUT_PIN 1
#else
    #error "No config for this target"
#endif

// Check if button pulldown should be enabled 
// Default to True as this was the default prior to needing to disable it
// in order to use the boot1 pin on the Blue Pill which has a very week pullup
#ifndef BUTTON_INPUT_MODE
	#define BUTTON_INPUT_MODE 	CR_INPUT_PU_PD
#endif

#define STARTUP_BLINKS 5
#ifndef BOOTLOADER_WAIT
#ifdef BUTTON_BANK
    #define BOOTLOADER_WAIT 6
#else
    #define BOOTLOADER_WAIT 30
#endif
#endif

// defines for USB (DONT CHANGE)
#define VEND_ID0 0x62
#define VEND_ID1 0x6B
#define PROD_ID0 0x00
#define PROD_ID1 0x00

// Value to place in RTC backup register 10 for persistent bootloader mode
#define RTC_BOOTLOADER_FLAG 0x424C
#endif
