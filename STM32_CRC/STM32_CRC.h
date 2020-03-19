/*
 *         _____          ___           ___           ___           ___                       ___           ___
 *        /  /::\        /  /\         /  /\         /  /\         /__/\          ___        /  /\         /__/|
 *       /  /:/\:\      /  /::\       /  /:/_       /  /::\        \  \:\        /  /\      /  /:/_       |  |:|
 *      /  /:/  \:\    /  /:/\:\     /  /:/ /\     /  /:/\:\        \  \:\      /  /:/     /  /:/ /\      |  |:|
 *     /__/:/ \__\:|  /  /:/  \:\   /  /:/ /::\   /  /:/  \:\   _____\__\:\    /  /:/     /  /:/ /:/_   __|  |:|
 *     \  \:\ /  /:/ /__/:/ \__\:\ /__/:/ /:/\:\ /__/:/ \__\:\ /__/::::::::\  /  /::\    /__/:/ /:/ /\ /__/\_|:|____
 *      \  \:\  /:/  \  \:\ /  /:/ \  \:\/:/~/:/ \  \:\ /  /:/ \  \:\~~\~~\/ /__/:/\:\   \  \:\/:/ /:/ \  \:\/:::::/
 *       \  \:\/:/    \  \:\  /:/   \  \::/ /:/   \  \:\  /:/   \  \:\  ~~~  \__\/  \:\   \  \::/ /:/   \  \::/~~~~
 *        \  \::/      \  \:\/:/     \__\/ /:/     \  \:\/:/     \  \:\           \  \:\   \  \:\/:/     \  \:\
 *         \__\/        \  \::/        /__/:/       \  \::/       \  \:\           \__\/    \  \::/       \  \:\
 *                       \__\/         \__\/         \__\/         \__\/                     \__\/         \__\/
 *
 *
 * @ 名称: STM32_CRC.h
 * @ 描述:
 * @ 作者: Tomy
 * @ 日期: 2020年3月17日
 * @ 版本: V1.0
 * @ 历史: V1.0 2020年3月17日 Summary
 *
 * Copyright (c) 2020 DosonTek Electronics Co., Ltd. All rights reserved.
 */

#pragma once
#ifndef STM32_CRC_H_
#define STM32_CRC_H_

#include "STM32_ADC.h"

 /* 不同编译器 4 字节对齐适配 */
#if defined (__MINGW32__) /* MINGW32 Compiler */

#ifndef __ALIGN_END
#define __ALIGN_END    __attribute__ ((aligned (4)))
#endif

#ifndef __ALIGN_BEGIN  
#define __ALIGN_BEGIN
#endif

#elif defined (_MSC_VER) /* MSVC Compiler */

#ifndef __ALIGN_END
#define __ALIGN_END
#endif

#ifndef __ALIGN_BEGIN      
#define __ALIGN_BEGIN   __declspec(align(4))
#endif

#endif

#define USB_DATA_OUT_BUFFER_SIZE    (8U)        /* BUFFER_SIZE 必须为 4 的整数倍 */
#define USB_DATA_IN_BUFFER_SIZE     (64U)       /* BUFFER_SIZE 必须为 4 的整数倍 */
#define CVT_TO_WORD_LENGTH(A)       ((A) / 4U)

/* sizeof(USB_DataOutPkt_t) 必须等于 USB_DATA_OUT_BUFFER_SIZE */
typedef struct {
    uint16_t    PayloadLen;
    uint8_t     Cmd[2];
    uint32_t    CRC_32;
} USB_DataOutPkt_t;

/* sizeof(USB_DataInPkt_t) 必须等于 USB_DATA_IN_BUFFER_SIZE */
typedef struct {
    uint16_t PayloadLen;

    struct {
        uint16_t ADC_SampleValArr[NUM_OF_CHN_TOTAL];

        /* USB_DATA_IN_BUFFER_SIZE 减去其它成员所占空间即为所需保留的字节数 */
        uint8_t rsvdData[USB_DATA_IN_BUFFER_SIZE - sizeof(uint16_t) - NUM_OF_CHN_TOTAL * 2 - sizeof(uint32_t)];
    } DataPayload_t;

    uint32_t CRC_32;
} USB_DataInPkt_t;

extern uint8_t* const pUSB_DataOutBuffer;
extern uint8_t* const pUSB_DataInBuffer;

#endif /* STM32_CRC_H_ */
