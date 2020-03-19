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
 * @ 名称: STM32_CRC.cpp
 * @ 描述:
 * @ 作者: Tomy
 * @ 日期: 2020年3月17日
 * @ 版本: V1.0
 * @ 历史: V1.0 2020年3月17日 Summary
 *
 * Copyright (c) 2020 DosonTek Electronics Co., Ltd. All rights reserved.
 */
#include <iostream>
#include "STM32_CRC.h"


/* 强制 4 字节对齐 */
/* DataOut 和 DataIn 是相对于 USB Host 而言的 */
__ALIGN_BEGIN uint8_t USB_DataOutBuffer[USB_DATA_OUT_BUFFER_SIZE] __ALIGN_END = {0}; /* DataOut 为 Host 向 Device 发出命令 */
__ALIGN_BEGIN uint8_t USB_DataInBuffer[USB_DATA_IN_BUFFER_SIZE] __ALIGN_END = {0}; /* DataIn 为 Device 向 Host 汇报数据 */

uint8_t* const pUSB_DataOutBuffer = USB_DataOutBuffer;
uint8_t* const pUSB_DataInBuffer = USB_DataInBuffer;

USB_DataOutPkt_t* const pUSB_DataOutPkt = (USB_DataOutPkt_t*)USB_DataOutBuffer;
USB_DataInPkt_t* const pUSB_DataInPkt = (USB_DataInPkt_t*)USB_DataInBuffer;

/* 用于调试中查看结构体所占内存空间大小、是否对齐 */
//USB_DataOutPkt_t USB_DataOutPkt = { 0xFF };
//USB_DataInPkt_t USB_DataInPkt = { 0xFF };
//uint8_t StructSize_1 = sizeof(USB_DataOutPkt_t);
//uint8_t StructSize_2 = sizeof(USB_DataOutPkt);
//uint8_t StructSize_3 = sizeof(USB_DataInPkt_t);
//uint8_t StructSize_4 = sizeof(USB_DataInPkt);

ADC_SampleVal_t* const pADC_SampleVal = (ADC_SampleVal_t*)pUSB_DataInPkt->DataPayload_t.ADC_SampleValArr;

uint32_t STM32_CRC_Calculate(uint32_t* pBuffer, uint32_t BufferLength)
{
    const uint32_t wST_HW_Poly = 0x04C11DB7;
    uint32_t wCRCVal = 0xFFFFFFFF;
    uint32_t xbit;
    uint32_t bits;
    uint32_t i;

    for (i = 0; i < BufferLength; i++)
    {
        xbit = 0x80000000;
        for (bits = 0; bits < 32; bits++)
        {
            if (wCRCVal & 0x80000000)
            {
                wCRCVal <<= 1;
                wCRCVal ^= wST_HW_Poly;
            }
            else
            {
                wCRCVal <<= 1;
            }
            if (pBuffer[i] & xbit)
            {
                wCRCVal ^= wST_HW_Poly;
            }
            xbit >>= 1;
        }
    }
    return wCRCVal;
}

int main()
{
    uint32_t CRC_32 = NULL;

    /* STEP 1: USB 接收，赋值给 pUSB_DataInBuffer 所指向的缓冲区 */
    /* TODO Something... */

    /* STEP 2: 接收后首先对 CRC 进行校验 */
    /* 注意传入的缓冲将以 uint32_t * 类型的指针进行访问，访问步长为 4 字节，故第二个形参指的是有多少个 uint32_t 类型的参数，而非多少个字节 */
    CRC_32 = STM32_CRC_Calculate((uint32_t *)pUSB_DataInPkt, CVT_TO_WORD_LENGTH(USB_DATA_IN_BUFFER_SIZE - sizeof(uint32_t)));
    if (CRC_32 == pUSB_DataInPkt->CRC_32)
    {
        /* CRC 校验通过 */
        printf("%s\r\n", pUSB_DataInBuffer);
        printf("0x%08X\r\n", CRC_32);
    }
    else
    {
        /* CRC 校验错误 */
    }    
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
