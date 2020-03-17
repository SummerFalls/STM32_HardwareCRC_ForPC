// STM32_CRC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

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
#define CVT_TO_WORD_LENGTH(A)       (A / 4U)

/* 强制 4 字节对齐 */
/* DataOut 和 DataIn 是相对于 USB Host 而言的 */
__ALIGN_BEGIN uint8_t USB_DataOutBuffer[USB_DATA_OUT_BUFFER_SIZE] __ALIGN_END = {0}; /* DataOut 为 Host 向 Device 发出命令 */
__ALIGN_BEGIN uint8_t USB_DataInBuffer[USB_DATA_IN_BUFFER_SIZE] __ALIGN_END = {0}; /* DataIn 为 Device 向 Host 汇报数据 */

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
    uint32_t CRCVal = 0;

    /* 注意传入的缓冲将以 uint32_t * 类型的指针进行访问，访问步长为 4 字节，故第二个形参指的是有多少个 uint32_t 类型的参数，而非多少个字节 */
    CRCVal = STM32_CRC_Calculate((uint32_t *)USB_DataOutBuffer, CVT_TO_WORD_LENGTH(USB_DATA_OUT_BUFFER_SIZE));
    printf("%s\r\n", USB_DataOutBuffer);
    printf("0x%08X\r\n", CRCVal);
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
