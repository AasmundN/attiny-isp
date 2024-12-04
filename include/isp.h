#ifndef __ISP__
#define __ISP__

#include <stdint.h>

typedef enum
{
  ISP_SUCCESS,
  ISP_ERROR,
  ISP_INVALID_CMD,
} isp_status_t;

typedef enum
{
  ISP_INACTIVE,
  ISP_ACTIVE,
} isp_state_t;

typedef enum
{
  ISP_ERASE = 1,
  ISP_VERIFY = 2,
  ISP_ERASE_VERIFY = 3,
} isp_arg_t;

void ISP_init(void);

isp_status_t ISP_write_program(uint8_t args, uint16_t program[], int size);

isp_status_t ISP_programming_enable(void);

isp_status_t ISP_chip_erase(void);

isp_status_t ISP_write_program_memory(uint8_t word_addr, uint16_t instruction);

isp_status_t ISP_read_program_memory(uint8_t word_addr, uint16_t *value);

#endif // __ISP__
