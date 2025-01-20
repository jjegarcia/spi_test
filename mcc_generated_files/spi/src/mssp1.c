/**
 * SPI1 Generated Driver File
 *
 * @file mssp1.c
 *
 * @ingroup spi1
 *
 * @brief This file contains the driver code for the SPI1 module.
 *
 * @version SPI1 Driver Version v5.0.0
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

#include <xc.h>
#include "../mssp1.h"
#include "../spi_interrupt_types.h"

const struct SPI_INTERFACE SPI1_Client = {
    .Initialize = SPI1_Initialize,
    .Deinitialize = SPI1_Deinitialize,
    .Open = SPI1_Open,
    .Close = SPI1_Close,
    .BufferExchange = SPI1_BufferExchange,
    .BufferWrite = SPI1_BufferWrite,
    .BufferRead = SPI1_BufferRead,
    .ByteExchange = SPI1_ByteExchange,
    .ByteWrite = SPI1_ByteWrite,
    .ByteRead = SPI1_ByteRead,
    .IsRxReady = SPI1_IsRxReady,
    .IsTxReady = SPI1_IsTxReady,
    .RxCompleteCallbackRegister = SPI1_RxCompleteCallbackRegister,
    .TxCompleteCallbackRegister = NULL
};

static void (*SPI1_RxCompleteCallback)(void);

static spi_descriptor_t spi1_descriptor = {
    .buffer         = NULL,
    .bufferLength   = 0,
    .status         = SPI_RESET
};

static const spi_configuration_t spi1_configuration[] = {
    { 0x40, 0x4, 0x10, 0x01 },
    { 0x0, 0x4, 0x10, 0x01 },
    { 0x40, 0x14, 0x10, 0x01 },
    { 0x0, 0x14, 0x10, 0x01 },
    { 0x64, 0x4, 0x10, 0x0 }
};

void SPI1_Initialize(void)
{
    // Return mssp1 registers to reset state
    PIE1bits.SSP1IE = 0;
    PIR1bits.SSP1IF = 0;

    SSP1STAT = 0x00;
    SSP1CON1 = 0x00;
    SSP1CON3 = 0x00;
    SSP1ADD = 0x00;

    // Clear Interrupt handler address
    SPI1_RxCompleteCallbackRegister(NULL);

    spi1_descriptor.status = SPI_RESET;
}

void SPI1_Deinitialize(void)
{
    // Return mssp1 registers to reset state
    SSP1STAT = 0x00;
    SSP1CON1 = 0x00;
    SSP1CON3 = 0x00;
    SSP1ADD  = 0x00;

    spi1_descriptor.status = SPI_RESET;
}

bool SPI1_Open(uint8_t spiConfigIndex)
{
    bool returnValue = false;
    if (SSP1CON1bits.SSPEN == false)
    {
        spi1_descriptor.status = SPI_IDLE;

        SSP1STAT = spi1_configuration[spiConfigIndex].stat;
        SSP1CON1 = spi1_configuration[spiConfigIndex].con1;
        SSP1CON3 = spi1_configuration[spiConfigIndex].con3;
        SSP1ADD  = spi1_configuration[spiConfigIndex].baud;

        SSP1CON1bits.SSPEN = 1;
        PIE1bits.SSP1IE = 0;

        returnValue = true;
    }
    else
    {
        returnValue = false;
    }
    return returnValue;
}

void SPI1_Close(void)
{
    SSP1CON1bits.SSPEN = 0;
    PIE1bits.SSP1IE = 0;
    PIR1bits.SSP1IF = 0;
    spi1_descriptor.status = SPI_RESET;
}

void SPI1_BufferExchange(void *bufferData, size_t bufferSize)
{
    uint8_t *bufferInput = bufferData;
    if (spi1_descriptor.status == SPI_IDLE)
    {
        PIE1bits.SSP1IE = 1;
        spi1_descriptor.buffer = bufferInput;
        spi1_descriptor.bufferLength = bufferSize - (size_t)1U;
        spi1_descriptor.transferType = SPI_EXCHANGE;
        spi1_descriptor.status = SPI_BUSY;

        SSP1BUF = *spi1_descriptor.buffer;
    }
    else
        {
        // No operation when module is already in use
        }
    }

void SPI1_BufferWrite(void *bufferData, size_t bufferSize)
{
    uint8_t *bufferInput = bufferData;
    if (spi1_descriptor.status == SPI_IDLE)
    {
        PIE1bits.SSP1IE = 1;
        spi1_descriptor.buffer = bufferInput;
        spi1_descriptor.bufferLength = bufferSize - (size_t)1U;
        spi1_descriptor.transferType = SPI_WRITE;
        spi1_descriptor.status = SPI_BUSY;

        SSP1BUF = *spi1_descriptor.buffer;
        }
    }

void SPI1_BufferRead(void *bufferData, size_t bufferSize)
{
    uint8_t *bufferInput = bufferData;
    if (spi1_descriptor.status == SPI_IDLE)
    {
        PIE1bits.SSP1IE = 1;
        spi1_descriptor.buffer = bufferInput;
        spi1_descriptor.bufferLength = bufferSize - (size_t)1U;
        spi1_descriptor.transferType = SPI_READ;
        spi1_descriptor.status = SPI_BUSY;

        SSP1BUF = 0x00;
    }
    else
        {
        // No operation when module is already in use
        }
    }

uint8_t SPI1_ByteExchange(uint8_t byteData)
{
    while (spi1_descriptor.status != SPI_IDLE)
    {
        // Waiting for the current operation to finish
    }

    PIE1bits.SSP1IE = 1;
    spi1_descriptor.buffer = &byteData;
    spi1_descriptor.bufferLength = 0;
    spi1_descriptor.transferType = SPI_EXCHANGE;
    spi1_descriptor.status = SPI_BUSY;

    SSP1BUF = *spi1_descriptor.buffer;
    while (spi1_descriptor.status == SPI_BUSY)
    {
        // Do nothing
}

    return *spi1_descriptor.buffer;
}

void SPI1_ByteWrite(uint8_t byteData)
{
    if (spi1_descriptor.status == SPI_IDLE)
    {
    SSP1BUF = byteData;
}
    else
    {
        // No operation when module is already in use
    }
}

uint8_t SPI1_ByteRead(void)
{
    uint8_t returnValue = 0;
    if (spi1_descriptor.status == SPI_IDLE)
    {
        returnValue = SSP1BUF;
    if (1U == PIR1bits.SSP1IF)
    {
        PIR1bits.SSP1IF = 0;
    }
}
    else
    {
        // No operation when module is already in use
    }
    return returnValue;
}

bool SPI1_IsRxReady(void)
{
    bool returnValue = false;
    if ((spi1_descriptor.status == SPI_IDLE) && (1U == PIR1bits.SSP1IF))
    {
        returnValue = true;
    }
    else
    {
        returnValue = false;
    }
    return returnValue;
}

bool SPI1_IsTxReady(void)
{
    bool returnValue = false;
    if (spi1_descriptor.status == SPI_IDLE)
    {
        returnValue = true;
    }
    else
    {
        returnValue = false;
    }
    return returnValue;
}

void SPI1_ISR(void)
{
    PIR1bits.SSP1IF = 0;
    uint8_t dataToWrite;
    dataToWrite = 0x00;

    if (spi1_descriptor.bufferLength > (size_t)0)
    {
        if (spi1_descriptor.transferType != SPI_WRITE)
        {
            *spi1_descriptor.buffer = SSP1BUF;
        }
        else
        {
            // transferType is SPI_WRITE
        }

        ++spi1_descriptor.buffer;

        if (spi1_descriptor.transferType != SPI_READ)
        {
            dataToWrite = *spi1_descriptor.buffer;
        }
        else
        {
            // transferType is SPI_READ
        }
        --spi1_descriptor.bufferLength;
        SSP1BUF = dataToWrite;
    }
    else
    {
        if ((spi1_descriptor.transferType != SPI_WRITE) && (spi1_descriptor.status != SPI_IDLE))
        {
            *spi1_descriptor.buffer = SSP1BUF;
        }
        else
        {
            // transferType SPI_WRITE and no more writes are required
        }

        spi1_descriptor.status = SPI_IDLE;
        PIE1bits.SSP1IE = 0;

        if (SPI1_RxCompleteCallback != NULL)
        {
            SPI1_RxCompleteCallback();
        }
        else
        {
            // No callback exists
        }
    }
}

void SPI1_RxCompleteCallbackRegister(void (*CallbackHandler)(void))
{
    SPI1_RxCompleteCallback = CallbackHandler;
}