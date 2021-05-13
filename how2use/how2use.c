/**
 * @file test.c
 * @author vzican (vvzpumpkin@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-05-13
 *
 * @copyright Copyright (c) 2021 vzican@github.com
 *
 * @licence: MIT license
 */

#include "ringbuffer.h"
#include "stdint.h"
#include "stdio.h"

void dump(uint8_t *data, uint32_t size) {
  for (uint32_t i; i < size; i++) printf("[%d]", data[i]);

  printf("\n");
}

int main(void) {
  uint8_t input[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  uint8_t output[10];
  // uint8_t takeOverBuffer[10] = {10,9,8,7,6,5,4,3,2,1};
  Ringbuffer_StatusEnum retStatus;

  Ringbuffer_InfoType create_test_rbinfo;
  // Ringbuffer_InfoType takeover_test_rbinfo;

  retStatus = ringBuffer_Create(&create_test_rbinfo, 10);
  printf("ringBuffer_Create return [%d]\n", retStatus);

  // retStatus = ringBuffer_TakeOver(&takeover_test_rbinfo, takeOverBuffer, 10);
  // printf("ringBuffer_Create return [%d]\n", retStatus);

  // retStatus = ringBuffer_Push(&takeover_test_rbinfo, input, 5);
  retStatus = ringBuffer_Push(&create_test_rbinfo, input, 5);
  printf("ringBuffer_Push return [%d]\n", retStatus);
  dump(input, 10);

  // retStatus = ringBuffer_Pop(&takeover_test_rbinfo, output, 7);
  retStatus = ringBuffer_Pop(&create_test_rbinfo, output, 7);
  printf("ringBuffer_Pop return [%d]\n", retStatus);
  dump(output, 10);

  // retStatus = ringBuffer_Pop(&takeover_test_rbinfo, output, 3);
  retStatus = ringBuffer_Pop(&create_test_rbinfo, output, 3);
  printf("ringBuffer_Pop return [%d]\n", retStatus);
  dump(output, 10);

  // retStatus = ringBuffer_Push(&takeover_test_rbinfo, input, 10);
  retStatus = ringBuffer_Push(&create_test_rbinfo, input, 10);
  printf("ringBuffer_Push return [%d]\n", retStatus);
  dump(input, 10);

  // retStatus = ringBuffer_Pop(&takeover_test_rbinfo, output, 5);
  retStatus = ringBuffer_Pop(&create_test_rbinfo, output, 5);
  printf("ringBuffer_Pop return [%d]\n", retStatus);
  dump(output, 10);

  return 0;
}