#include "debug.h"

/**
 * By Matheus Braga Almeida
 * ch32v003 i2c scanner
 *
 * Make sure to add two resistors from vcc to on pc2 and pc1. It can be about 10k ohm
 * PC1 = SDA
 * PC2 = SLC
 *
 * This file is intended to be build in the MounRiver studio. Just create an empty project for the whatever version of the ch32v003 you like and replace the main.c with this file
 */

void SetupI2cPins()
{
    //Its necessary to setup the GPIOs for the i2c.
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );


}

void SetupI2c(u8 address){
    //This would be the arduino equivalent of Wire.beginTransmission(address);
    I2C_InitTypeDef I2C_InitTSturcture={0};
    I2C_InitTSturcture.I2C_ClockSpeed = 100000;  //100000 mhz ofc
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitTSturcture );
    I2C_Cmd( I2C1, ENABLE );
    I2C_AcknowledgeConfig( I2C1, ENABLE );
}




int CheckDevicePresence(u8 addr)
{
    //This is required because the i2c might still be doing something, so we have to wait.
    //If your code hangs on this function, make sure to close the i2c with I2C_GenerateSTOP or add an timeout
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    //Enables the i2c
    I2C_GenerateSTART( I2C1, ENABLE );
    //Makes sure it is enabled and ready to start acting as a master
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    //The address is 7 bit only, the last bit is the r/w bit. But this function already incudes for us this bit.
    //But that is the LAST bit, so lets say we have 1111.1111 as address and 1111.1110, both will show the very same device because this
    //function rips of the last bit. So to avoid any further confusion, i added the lshift operation, so now the addresses will be from 1 to 127 without repetition :3
    I2C_Send7bitAddress( I2C1, addr << 1, I2C_Direction_Transmitter );

    int cycles = 1000;
    //Once sent, we need to have an response from the slave device. Since this example covers just the i2c scan section, we need an "timeout" because most of addresses wont have a valid device
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ){
        cycles--;
        if (cycles <= 0){
            //Make sure to close the i2c
            I2C_GenerateSTOP( I2C1, ENABLE );
            return 0;
        }
    }

    I2C_GenerateSTOP( I2C1, ENABLE );
    return 1;
}

int main(void)
{

    Delay_Init();
    USART_Printf_Init(115200);
    SetupI2cPins();
    printf("SystemClk:%d\r\n",SystemCoreClock);

    printf("Starting I2c scanner\r\n");
    int devices = 0;
    for (int i=1;i<127;i++){
        SetupI2c(i);
        int found = CheckDevicePresence(i);
        if (found == 1){
            printf("Found in device in:0x%x: \r\n", i);
            devices++;
        }
    }

    printf("Found total of %d devices\r\n", devices);

    while(1);
}
