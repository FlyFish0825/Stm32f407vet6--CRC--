#include "iap.h"
#include "stm32f4xx_hal_def.h"

uint8_t IAP_Write_Flash(uint32_t address, uint8_t *data, uint16_t len) {
  HAL_FLASH_Unlock();
  while (len--) {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, *data) != HAL_OK) {
      HAL_FLASH_Lock();
      return HAL_ERROR;
    }
    address++;
    data++;
  }
  HAL_FLASH_Lock();

  return HAL_OK;
}