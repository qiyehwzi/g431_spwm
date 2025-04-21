
//#ifndef __ADS8688_H__
//#define __ADS8688_H__

//#ifdef __cplusplus
//extern "C" {
//#endif

//#include "stm32g4xx_hal.h"

//// 引脚定义
//#define ADS8688_CS_PORT    GPIOB
//#define ADS8688_CS_PIN     GPIO_PIN_4
//#define ADS8688_SCK_PORT   GPIOB
//#define ADS8688_SCK_PIN    GPIO_PIN_5
//#define ADS8688_MOSI_PORT  GPIOB
//#define ADS8688_MOSI_PIN   GPIO_PIN_6
//#define ADS8688_MISO_PORT  GPIOB
//#define ADS8688_MISO_PIN   GPIO_PIN_7

//// 命令定义
//#define ADS8688_CMD_AUTO_RST  0xA000 // Auto-reset模式命令
//#define ADS8688_CMD_READ      0xA800 // 通道0读取命令

//// 配置AUTO_SCAN_SEQ_1（通道0-7）
//#define AUTO_SCAN_SEQ_1  0x01FF  // 通道0-7使能
//#define AUTO_SCAN_SEQ_2  0x0000  // 未使用的高位通道

//typedef enum {
//    ADS8688_OK = 0,
//    ADS8688_ERROR
//} ADS8688_Status;

//void ADS8688_Init(void);
//ADS8688_Status ADS8688_ReadData(uint16_t *data);

//#ifdef __cplusplus
//}
//#endif

//#endif
