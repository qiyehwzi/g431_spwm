
//#include "ads8688.h"
//#include "main.h"
//#include "cmsis_os.h"

//#define DWT_CYCCNT  *(volatile uint32_t*)0xE0001004
//#define DWT_CONTROL *(volatile uint32_t*)0xE0001000
//#define SCB_DEMCR   *(volatile uint32_t*)0xE000EDFC

//static void DWT_Init(void) {
//    SCB_DEMCR   |= 0x01000000;
//    DWT_CONTROL |= 1;
//}

//static void SPI_Delay(uint32_t us) {
//    uint32_t start = DWT_CYCCNT;
//    uint32_t cycles = us * 150; // 150MHz主频
//    while((DWT_CYCCNT - start) < cycles);
//}

//static void CS_Enable(void) {
//    HAL_GPIO_WritePin(ADS8688_CS_PORT, ADS8688_CS_PIN, GPIO_PIN_RESET);
//    SPI_Delay(2); // 保持CS低电平至少2个SCLK周期
//}

//static void CS_Disable(void) {
//    SPI_Delay(2);
//    HAL_GPIO_WritePin(ADS8688_CS_PORT, ADS8688_CS_PIN, GPIO_PIN_SET);
////    osDelay(1); // 帧间隔时间
//}

//static uint8_t SPI_TransferByte(uint8_t txData) {
//    uint8_t rxData = 0;
//    for(uint8_t i = 0; i < 8; i++) {
//        // 下降沿准备数据
//        HAL_GPIO_WritePin(ADS8688_SCK_PORT, ADS8688_SCK_PIN, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(ADS8688_MOSI_PORT, ADS8688_MOSI_PIN, (txData & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//        txData <<= 1;
//			
//			  // 增加数据建立时间
//        SPI_Delay(1);  // 插入关键延时
//        
//        // 上升沿采样数据
//        HAL_GPIO_WritePin(ADS8688_SCK_PORT, ADS8688_SCK_PIN, GPIO_PIN_SET);
//        SPI_Delay(1); // 保持时钟高电平时间
//        
//        rxData <<= 1;
//        if(HAL_GPIO_ReadPin(ADS8688_MISO_PORT, ADS8688_MISO_PIN)) {
//            rxData |= 0x01;
//        }
//    }
//    return rxData;
//}

//void ADS8688_Init(void) {
//    DWT_Init();
//    
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    __HAL_RCC_GPIOB_CLK_ENABLE();

//    // 配置CS
//    GPIO_InitStruct.Pin = ADS8688_CS_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(ADS8688_CS_PORT, &GPIO_InitStruct);

//    // 配置SCK和MOSI
//    GPIO_InitStruct.Pin = ADS8688_SCK_PIN | ADS8688_MOSI_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(ADS8688_SCK_PORT, &GPIO_InitStruct);

//    // 配置MISO
//    GPIO_InitStruct.Pin = ADS8688_MISO_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    HAL_GPIO_Init(ADS8688_MISO_PORT, &GPIO_InitStruct);

//    CS_Disable();
//		
// // 发送硬件复位命令
//    uint16_t resetCmd = 0x8500;
//    CS_Enable();
//    SPI_TransferByte(resetCmd >> 8);
//    SPI_TransferByte(resetCmd & 0xFF);
//    CS_Disable();
//    HAL_Delay(10);
//		
//		// 配置自动扫描通道序列（启用通道0-7）
//    uint16_t autoScanSeq1 = 0x6100 | 0xFF; // 写寄存器0x01（地址0x61）
//    CS_Enable();
//    SPI_TransferByte(autoScanSeq1 >> 8);
//    SPI_TransferByte(autoScanSeq1 & 0xFF);
//    CS_Disable();
//    HAL_Delay(10);

//    // 配置通道0输入范围为±5V（VREF=4.096V）
//    uint16_t rangeCmd = 0x6500 | 0x02; // 寄存器0x6F，值0x05
//    CS_Enable();
//    SPI_TransferByte(rangeCmd >> 8);
//    SPI_TransferByte(rangeCmd & 0xFF);
//    CS_Disable();
//    HAL_Delay(10);

//    // 进入AUTO_RST模式
//    uint16_t autoRstCmd = 0xA000;
//    CS_Enable();
//    SPI_TransferByte(autoRstCmd >> 8);
//    SPI_TransferByte(autoRstCmd & 0xFF);
//    CS_Disable();

//}

//ADS8688_Status ADS8688_ReadData(uint16_t *data) {
//    uint8_t rxBuf[2] = {0};
//    
//    CS_Enable();  // 拉低CS触发数据传输
//    rxBuf[0] = SPI_TransferByte(0xFF); // 读取高8位
//    rxBuf[1] = SPI_TransferByte(0xFF); // 读取低8位
//    CS_Disable(); // 拉高CS结束传输
//    
//    *data = (rxBuf[0] << 8) | rxBuf[1];
//    return ADS8688_OK;
//}
