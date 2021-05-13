/**
 * @file ringbuffer.h
 * @author vzican (vvzpumpkin@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-05-13
 *
 * @copyright Copyright (c) 2021 vzican@github.com
 *
 * @licence: MIT license
 */

#include "stdint.h"

typedef struct {
  uint8_t takeFlag;  // Used to determine whether the ring buffer was created or
                     // taken over

  uint8_t *buffer;  // The address of the ring buffer in memory

  int32_t size;  // Size of the ring buffer

  int32_t remaning;  // remaning space

  uint32_t headp;  // Read pointer of ring buffer

  uint32_t tailp;  // Write pointer of ring buffer
} Ringbuffer_InfoType;

typedef enum {
  RINGBUFFER_SUCCESSFUL = 0,
  RINGBUFFER_FAILED,
  RINGBUFFER_EMPTY,
  RINGBUFFER_ERROR,
  RINGBUFFER_STATUE_END
} Ringbuffer_StatusEnum;

typedef enum {
  RINGBUFFER_GET_REMANING = 0,
  RINGBUFFER_GET_USED,
  RINGBUFFER_Get_END
} Ringbuffer_GetEnum;

typedef enum {
  RINGBUFFER_MEM_CREATE = 0,
  RINGBUFFER_MEM_TAKEOVER,
  RINGBUFFER_MEM_END
} Ringbuffer_MemEnum;

Ringbuffer_StatusEnum ringBuffer_Create(Ringbuffer_InfoType *psInfo,
                                        uint32_t size);

Ringbuffer_StatusEnum ringBuffer_TakeOver(Ringbuffer_InfoType *psInfo,
                                          uint8_t *addr, uint32_t size);

Ringbuffer_StatusEnum ringBuffer_Destroy(Ringbuffer_InfoType *psInfo);

Ringbuffer_StatusEnum ringBuffer_Clear(Ringbuffer_InfoType *psInfo);

Ringbuffer_StatusEnum ringBuffer_Push(Ringbuffer_InfoType *psInfo,
                                      uint8_t *pdata, uint32_t size);

Ringbuffer_StatusEnum ringBuffer_Pop(Ringbuffer_InfoType *psInfo,
                                     uint8_t *pdata, uint32_t size);

Ringbuffer_StatusEnum ringBuffer_Peek(Ringbuffer_InfoType *psInfo,
                                      uint8_t *pdata, uint32_t size);

Ringbuffer_StatusEnum ringBuffer_Get(Ringbuffer_InfoType *psInfo, uint32_t *out,
                                     Ringbuffer_GetEnum type);