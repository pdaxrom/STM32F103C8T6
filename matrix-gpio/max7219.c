#include "stm32f10x_lib.h"
#include "max7219.h"

typedef enum {
	REG_NO_OP 			= 0x00 << 8,
	REG_DIGIT_0 		= 0x01 << 8,
	REG_DIGIT_1 		= 0x02 << 8,
	REG_DIGIT_2 		= 0x03 << 8,
	REG_DIGIT_3 		= 0x04 << 8,
	REG_DIGIT_4 		= 0x05 << 8,
	REG_DIGIT_5 		= 0x06 << 8,
	REG_DIGIT_6 		= 0x07 << 8,
	REG_DIGIT_7 		= 0x08 << 8,
	REG_DECODE_MODE 	= 0x09 << 8,
	REG_INTENSITY 		= 0x0A << 8,
	REG_SCAN_LIMIT 		= 0x0B << 8,
	REG_SHUTDOWN 		= 0x0C << 8,
	REG_DISPLAY_TEST 	= 0x0F << 8,
} MAX7219_REGISTERS;

static void max7219_clean(void);
static void sendData(uint16_t data);

//static uint8_t framebuffer[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

void max7219_init(uint8_t intensivity)
{
/*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_InitTypeDef pins;
	SPI_InitTypeDef spi;

	pins.GPIO_Mode = GPIO_Mode_AF_PP;
	pins.GPIO_Speed = GPIO_Speed_50MHz;
	pins.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7; // MISO, SCK
	GPIO_Init(GPIOA, &pins);

	pins.GPIO_Mode = GPIO_Mode_Out_PP;
	pins.GPIO_Pin = GPIO_Pin_6; // CS
	GPIO_Init(GPIOA, &pins);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);

	spi.SPI_Direction = SPI_Direction_1Line_Tx;
	spi.SPI_DataSize = SPI_DataSize_16b;
	spi.SPI_CPOL = SPI_CPOL_Low;
	spi.SPI_CPHA = SPI_CPHA_1Edge;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	spi.SPI_Mode = SPI_Mode_Master;

	SPI_Init(SPI1, &spi);
	SPI_Cmd(SPI1, ENABLE);

	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
 */

	Delay(800);
	max7219_setIntensivity(intensivity);
	max7219_clean();
//	max7219_display(framebuffer);
}

void max7219_setIntensivity(uint8_t intensivity)
{
	if (intensivity > 0x0F)
		return;
	sendData(REG_SHUTDOWN | 0x01);
	sendData(REG_DECODE_MODE | 0x00);
	sendData(REG_SCAN_LIMIT | 0x07);
	sendData(REG_INTENSITY | intensivity);
}

static void max7219_clean()
{
	sendData(REG_DIGIT_0 | 0x00);
	sendData(REG_DIGIT_1 | 0x00);
	sendData(REG_DIGIT_2 | 0x00);
	sendData(REG_DIGIT_3 | 0x00);
	sendData(REG_DIGIT_4 | 0x00);
	sendData(REG_DIGIT_5 | 0x00);
	sendData(REG_DIGIT_6 | 0x00);
	sendData(REG_DIGIT_7 | 0x00);
}

static void sendData(uint16_t data)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
/*
	SPI_SendData(SPI1, data);
	while (SPI_GetFlagStatus(SPI1, SPI_FLAG_BSY) == SET);
	// asm("nop");
 */

	int i;
	for (i = 0; i < 16; i++) {
	    if (data & 0x8000) {
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
	    } else {
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	    }
	    GPIO_SetBits(GPIOA, GPIO_Pin_5);
//	    Delay(0x8);
	    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//	    Delay(0x8);
	    data <<= 1;
	}

	GPIO_SetBits(GPIOA, GPIO_Pin_6);
}

void max7219_display(uint8_t* array)
{
	sendData(REG_SHUTDOWN | 0x01);
	sendData(REG_DECODE_MODE | 0x00);
	sendData(REG_SCAN_LIMIT | 0x07);

	sendData(REG_DIGIT_0 | array[0]);
	sendData(REG_DIGIT_1 | array[1]);
	sendData(REG_DIGIT_2 | array[2]);
	sendData(REG_DIGIT_3 | array[3]);
	sendData(REG_DIGIT_4 | array[4]);
	sendData(REG_DIGIT_5 | array[5]);
	sendData(REG_DIGIT_6 | array[6]);
	sendData(REG_DIGIT_7 | array[7]);
}
