/*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
 */

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"

/*
    Main application
 */
bool led_state = false;
void toggle_led(void);
void spi_send_data(uint8_t *data);

int main(void) {
    SYSTEM_Initialize();
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable(); 
    uint8_t spiData[] = {
        0x29, 0x67, 0x77, 0x64
//                ,0x29, 0x67, 0x77, 0x64,
//        0x29, 0x67, 0x77, 0x64, 0x29, 0x67, 0x77, 0x64,
//        0x29, 0x67, 0x77, 0x64, 0x29, 0x67, 0x77, 0x64,
//        0x29, 0x67, 0x77, 0x64, 0x29, 0x67, 0x77, 0x64
    };

    while (1) {
        if (timerOverflow) {
            toggle_led();
            timerOverflow = false;
            //              spi_send_data(spiData);
        }
        //        if (pushed)
        {
            spi_send_data(&spiData);
            pushed = false;
        }
    }
}

void toggle_led(void) {
    led_state = ~led_state;
    LATAbits.LATA2 = led_state;
}

void spi_send_data(uint8_t *data) {
    //      uint8_t *bufferInput = data;

    if (SPI1_Open(0)) {
        while (!SPI1_IsTxReady()) {}
        SPI1_BufferWrite(data, 4);
        SPI1_Close();
    }
}