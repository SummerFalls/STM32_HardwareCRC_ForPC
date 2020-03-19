#pragma once
#ifndef STM32_ADC_H_
#define STM32_ADC_H_

#if defined (__MINGW32__) /* MINGW32 Compiler */

#elif defined (_MSC_VER) /* MSVC Compiler */
#include <cstdint>
#endif

/* TODO 00: 对应 CubeMX 中的 Number Of Conversion */
#define NUM_OF_CHN_ADC1     14U
#define NUM_OF_CHN_ADC3      4U

#define NUM_OF_CHN_TOTAL    (NUM_OF_CHN_ADC1 + NUM_OF_CHN_ADC3)

/* TODO 01: 参数排列顺序受到 CubeMX 中的 Rank 影响，需要保持一致 */
/* NOTE: NUM_OF_CHN_TOTAL = ADC1 + ADC3 */
typedef struct {
    /* ADC1 */
    uint16_t TEST_SPD_VR_IN;
    uint16_t TEST_FAULT_IN;
    uint16_t TEST_NTC_IN;
    uint16_t TEST_CUR_BUS_IN;
    uint16_t TEST_CUR_U_IN;
    uint16_t TEST_CUR_V_IN;
    uint16_t FCT_BUS_CUR;
    uint16_t FCT_DC_VOL;
    uint16_t TEST_10V_IN;
    uint16_t TEST_5V_IN;
    uint16_t TEST_3V3_IN;
    uint16_t TEST_1V65_IN;
    uint16_t TEST_CUR_W_IN;
    uint16_t FCT_NTC;

    /* ADC3 */
    uint16_t TEST_12V_IN;
    uint16_t TEST_VP_COM;
    uint16_t TEST_HVP_IN;
    uint16_t TEST_15V_IN;
} ADC_SampleVal_t;

#endif /* STM32_ADC_H_ */
