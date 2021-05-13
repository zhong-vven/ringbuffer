/**
 * @file ringbuffer.c
 * @author vzican (vvzpumpkin@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-05-13
 *
 * @copyright Copyright (c) 2021 vziCan@https://github.com/vziCan
 *
 * @licence: MIT license
 */

#include "ringbuffer.h"

#include "string.h"

/**
 * @brief
 *  Create space for ring buffer
 *
 * @param psInfo : Store infomation about the ring buffer
 * @param size : The size of the ring buffer in bytes
 * @return Ringbuffer_StatusEnum : return operation status
 */
Ringbuffer_StatusEnum ringBuffer_Create(Ringbuffer_InfoType *psInfo,
                                        uint32_t size) {
  if (psInfo == NULL) return RINGBUFFER_FAILED;

  psInfo->takeFlag = RINGBUFFER_MEM_CREATE;
  psInfo->buffer = (uint8_t *)malloc(size);
  psInfo->size = size;
  psInfo->remaning = size;
  psInfo->headp = 0;
  psInfo->tailp = 0;

  return RINGBUFFER_SUCCESSFUL;
}

/**
 * @brief
 *  Take over the existing space
 *
 * @param psInfo : Store infomation about the ring buffer
 * @param addr : Take over the given address area for ring buffer
 * @param size : The size of the ring buffer in bytes
 * @return Ringbuffer_StatusEnum : return operation status
 */
Ringbuffer_StatusEnum ringBuffer_TakeOver(Ringbuffer_InfoType *psInfo,
                                          uint8_t *addr, uint32_t size) {
  if ((psInfo == NULL) && (addr == NULL)) return RINGBUFFER_FAILED;

  psInfo->takeFlag = RINGBUFFER_MEM_TAKEOVER;
  psInfo->buffer = addr;
  psInfo->size = size;
  psInfo->remaning = size;
  psInfo->headp = 0;
  psInfo->tailp = 0;

  return RINGBUFFER_SUCCESSFUL;
}

/**
 * @brief
 *  Free space and clearn info
 *
 * @param psInfo : Store infomation about the ring buffer
 * @return Ringbuffer_StatusEnum : return operation status
 */
Ringbuffer_StatusEnum ringBuffer_Destroy(Ringbuffer_InfoType *psInfo) {
  if (psInfo && psInfo->buffer)
    if (psInfo->takeFlag == RINGBUFFER_MEM_CREATE) free(psInfo->buffer);

  memset(psInfo, 0, sizeof(Ringbuffer_InfoType));

  return RINGBUFFER_SUCCESSFUL;
}

/**
 * @brief
 *  Description of reset ring buffer
 *
 * @param psInfo : Store infomation about the ring buffer
 * @return Ringbuffer_StatusEnum : return operation status
 */
Ringbuffer_StatusEnum ringBuffer_Clear(Ringbuffer_InfoType *psInfo) {
  if ((psInfo == NULL) && (psInfo->buffer == NULL)) return RINGBUFFER_ERROR;

  psInfo->remaning = psInfo->size;
  psInfo->headp = 0;
  psInfo->tailp = 0;

  return RINGBUFFER_SUCCESSFUL;
}

/**
 * @brief
 *  Push data to ring buffer
 *
 *  ps: If the buffer is full, the old data will be overwritten
 *
 * @param psInfo : Store infomation about the ring buffer
 * @param pdata : Data source
 * @param size : The size of the data will push into
 * @return Ringbuffer_StatusEnum : return operation status
 */
Ringbuffer_StatusEnum ringBuffer_Push(Ringbuffer_InfoType *psInfo,
                                      uint8_t *pdata, uint32_t size) {
  if ((psInfo == NULL) && (psInfo->buffer == NULL)) return RINGBUFFER_ERROR;

  if ((psInfo->tailp + size) <= psInfo->size) {
    memcpy(psInfo->buffer + psInfo->tailp, pdata, size);
  } else {
    uint32_t partSize = psInfo->size - psInfo->tailp;
    memcpy(psInfo->buffer + psInfo->tailp, pdata, partSize);
    memcpy(psInfo->buffer, pdata + partSize, size - partSize);
  }

  psInfo->tailp = (psInfo->tailp + size) % psInfo->size;

  if ((psInfo->tailp + size) / psInfo->size) psInfo->headp = psInfo->tailp;
  psInfo->remaning = psInfo->remaning - size < 0 ? psInfo->remaning - size : 0;
  // dbgPrint("pop[%d]-re[%d]-h[%d]-t[%d]-\n", size,
  // psInfo->remaning,psInfo->headp,psInfo->tailp);

  return RINGBUFFER_SUCCESSFUL;
}

/**
 * @brief
 *  Pop up data from ring buffer
 *
 *  ps: If there is not enough data, return to execution
 *       failure(RINGBUFFER_FAILED or RINGBUFFER_EMPTY)
 *
 * @param psInfo : Store infomation about the ring buffer
 * @param pdata : Where to store pop-up data
 * @param size : Pop up data of specified size
 * @return Ringbuffer_StatusEnum : return operation status
 */
Ringbuffer_StatusEnum ringBuffer_Pop(Ringbuffer_InfoType *psInfo,
                                     uint8_t *pdata, uint32_t size) {
  if ((psInfo == NULL) && (psInfo->buffer == NULL)) return RINGBUFFER_ERROR;

  if (psInfo->remaning == psInfo->size) return RINGBUFFER_EMPTY;

  if ((psInfo->size - psInfo->remaning) < size) return RINGBUFFER_FAILED;

  if ((psInfo->headp + size) <= psInfo->size) {
    memcpy(pdata, psInfo->buffer + psInfo->headp, size);
  } else {
    uint8_t partSize = psInfo->size - psInfo->headp;
    memcpy(pdata, psInfo->buffer + psInfo->headp, partSize);
    memcpy(pdata + partSize, psInfo->buffer, size - partSize);
  }

  psInfo->headp = (psInfo->headp + size) % psInfo->size;
  psInfo->remaning += size;
  // dbgPrint("pop[%d]-re[%d]-h[%d]-t[%d]-\n", size,
  // psInfo->remaning,psInfo->headp,psInfo->tailp);

  return RINGBUFFER_SUCCESSFUL;
}

/**
 * @brief
 *  View the data in the buffer, but do not pop out
 *
 * @param psInfo : Store infomation about the ring buffer
 * @param pdata : Where to store pop-up data
 * @param size : Pop up data of specified size
 * @return Ringbuffer_StatusEnum : return operation status
 */
Ringbuffer_StatusEnum ringBuffer_Peek(Ringbuffer_InfoType *psInfo,
                                      uint8_t *pdata, uint32_t size) {
  if ((psInfo == NULL) && (psInfo->buffer == NULL)) return RINGBUFFER_ERROR;

  if (psInfo->size < size) return RINGBUFFER_FAILED;

  if ((psInfo->headp + size) <= psInfo->size) {
    memcpy(pdata, psInfo->buffer + psInfo->headp, size);
  } else {
    uint8_t partSize = psInfo->size - psInfo->headp;
    memcpy(pdata, psInfo->buffer + psInfo->headp, partSize);
    memcpy(pdata + partSize, psInfo->buffer, size - partSize);
  }

  // dbgPrint("peek[%d]-re[%d]-h[%d]-t[%d]-\n", size,
  // psInfo->remaning,psInfo->headp,psInfo->tailp);

  return RINGBUFFER_SUCCESSFUL;
}

/**
 * @brief
 *  Get current information about the buffer
 *
 * @param psInfo : Store infomation about the ring buffer
 * @param out :
 * @param type :
 * @return Ringbuffer_StatusEnum : return operation status
 */
Ringbuffer_StatusEnum ringBuffer_Get(Ringbuffer_InfoType *psInfo, uint32_t *out,
                                     Ringbuffer_GetEnum type) {
  if ((psInfo == NULL) && (psInfo->buffer == NULL)) return RINGBUFFER_ERROR;

  if (type == RINGBUFFER_GET_REMANING)
    *out = psInfo->remaning;
  else if (type == RINGBUFFER_GET_USED)
    *out = psInfo->size - psInfo->remaning;

  return RINGBUFFER_SUCCESSFUL;
}