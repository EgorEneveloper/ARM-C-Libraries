#include "stm32f10x.h"
#include "utils.h"

#define F_CPU 8000000UL

#define PRESC_ms (F_CPU / 1000)
#define PRESC_us (F_CPU / 1000000)

void _delay_cycles(volatile unsigned cycles) { while (--cycles); }
void _delay_us(volatile unsigned us) { us *= PRESC_us; while(--us); }
void _delay_ms(volatile unsigned ms) { ms *= PRESC_ms; while(--ms); }

void SysClockConfig(void) {
    RCC->CR |= RCC_CR_CSSON;            //ENABLE HSE
    while(!(RCC->CR & RCC_CR_HSERDY));  //Wait till it ready

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;  //ENABLE power

    FLASH->ACR |= FLASH_ACR_LATENCY_0;  //Setup FLASH

    //PRESCALERS
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    //AHB
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;   //APB1
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;   //APB2

    RCC->CR |= RCC_CR_PLLON;            //ENABLE PLL
    while(!(RCC->CR & RCC_CR_PLLRDY));  //Wait till it ready

    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    //SysTick_Config(SystemCoreClock / 1000);
}
