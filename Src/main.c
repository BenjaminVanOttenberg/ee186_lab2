

#include <stdint.h>


#define gpio_cfg_mode_input 0b00
#define gpio_cfg_mode_gen_output 0b01
#define gpio_cfg_mode_alt_function 0b10
#define gpio_cfg_mode_analog 0b11

#define gpio_otype_push_pull 0b0
#define gpio_otype_open_drain 0b1

#define gpio_ospeed_low 0b00
#define gpio_ospeed_medium 0b01
#define gpio_ospeed_high 0b10
#define gpio_ospeed_very_high 0b11

#define gpio_pupd_none 0b00
#define gpio_pupd_pull_up 0b01
#define gpio_pupd_pull_down 0b10


#define RCC_BASE 0x40021000
#define RCC_AHB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x60))

// define GPIO registers for control of blue user LED #2
#define GPIO_BASEB 0x48000400

#define GPIOB_MODER   ( *(volatile uint32_t *)(GPIO_BASEB + 0x00) )
#define GPIOB_OTYPER  ( *(volatile uint32_t *)(GPIO_BASEB + 0x04) )
#define GPIOB_OSPEEDR ( *(volatile uint32_t *)(GPIO_BASEB + 0x08) )
#define GPIOB_PUPDR   ( *(volatile uint32_t *)(GPIO_BASEB + 0x0C) )

#define LED1_num 7
#define LED2_num 7
#define LED3_num 14

#define GPIOB_ODR ( *(volatile uint32_t *)(GPIO_BASEB + 0x14) )

// define configuration registers for operation of user button 1
#define GPIO_BASEC 0x48000800

#define GPIOC_MODER   ( *(volatile uint32_t *)(GPIO_BASEC + 0x00) )
#define GPIOC_OTYPER  ( *(volatile uint32_t *)(GPIO_BASEC + 0x04) )
#define GPIOC_OSPEEDR ( *(volatile uint32_t *)(GPIO_BASEC + 0x08) )
#define GPIOC_PUPDR   ( *(volatile uint32_t *)(GPIO_BASEC + 0x0C) )

#define BUTTON_pin 13
#define GPIOC_ODR ( *(volatile uint32_t *)(GPIO_BASEC + 0x14) )
#define GPIOC_IDR ( *(volatile uint32_t *)(GPIO_BASEC + 0x10) )

// interrupt
#define SYSCFG_BASE    0x40010000
#define SYSCFG_EXTICR4 (*(volatile uint32_t*)(SYSCFG_BASE + 0x14))

// #define EXTI_BASE 	   (*(volatile uint32_t*)(0x40010400))
#define EXTI_BASE 0x40010400
#define EXTI_IMR1      (*(volatile uint32_t*)(EXTI_BASE + 0x00))
#define EXTI_RTSR1     (*(volatile uint32_t*)(EXTI_BASE + 0x08))
#define EXTI_FTSR1     (*(volatile uint32_t*)(EXTI_BASE + 0x0C))
#define EXTI_PR1       (*(volatile uint32_t*)(EXTI_BASE + 0x14))

#define NVIC_ISER0 (*(volatile uint32_t*)(0xE000E100))
#define NVIC_ISER1 (*(volatile uint32_t*)(0xE000E104))

volatile int LED_State = 0;  // 0 = off, 1 = on
volatile int LED_cycle_state = 0; // 0 = yellow, = blue, 2 = red


// TIMER

#define TIM4_BASE 0x40000800
#define TIM4_CR1 (*(volatile uint32_t*)(TIM4_BASE + 0x00)) // control register 1
#define TIM4_PSC (*(volatile uint32_t*)(TIM4_BASE + 0x28)) // prescaler
#define TIM4_ARR (*(volatile uint32_t*)(TIM4_BASE + 0x2C)) // auto-reload register
#define TIM4_EGR (*(volatile uint32_t*)(TIM4_BASE + 0x14)) // event-generation register
#define TIM4_DIER (*(volatile uint32_t*)(TIM4_BASE + 0x0C)) // DMA/interrupt enable register
#define TIM4_SR (*(volatile uint32_t*)(TIM4_BASE + 0x10)) // DMA/interrupt enable register


#define RCC_CR (*(volatile uint32_t*)(RCC_BASE + 0x00))
#define RCC_CFGR (*(volatile uint32_t*)(RCC_BASE + 0x08))
#define RCC_APB1ENR1 (*(volatile uint32_t*)(RCC_BASE + 0x58))



int main(void) {
	// your code goes here
	RCC_AHB2ENR |= (1u << 1); // emable gpio port B clock
	RCC_AHB2ENR |= (1u << 2); // enable gpio port C clock
	RCC_APB2ENR |= (1u << 0); // Enable SYSCFG clock
	RCC_APB1ENR1 |= (1u << 2); // Enable timer 2 clock


	// CONFIGURE LED1 OUTPUT
	GPIOC_MODER &= ~(0x3 << (LED1_num*2));
	GPIOC_MODER |= (0x1 << (LED1_num*2));
	GPIOC_OTYPER &= ~(1u << LED1_num);
	GPIOC_OSPEEDR &= ~(0x3 << (LED1_num * 2));
	GPIOC_PUPDR &= ~(0x3 << (LED1_num * 2));

	// CONFIGURE LED2 OUTPUT
	GPIOB_MODER &= ~(0x3 << (LED2_num*2));
	GPIOB_MODER |= (0x1 << (LED2_num*2));
	GPIOB_OTYPER &= ~(1u << LED2_num);
	GPIOB_OSPEEDR &= ~(0x3 << (LED2_num * 2));
	GPIOB_PUPDR &= ~(0x3 << (LED2_num * 2));

	// CONFIGURE LED3 OUTPUT
	GPIOB_MODER &= ~(0x3 << (LED3_num*2));
	GPIOB_MODER |= (0x1 << (LED3_num*2));
	GPIOB_OTYPER &= ~(1u << LED3_num);
	GPIOB_OSPEEDR &= ~(0x3 << (LED3_num * 2));
	GPIOB_PUPDR &= ~(0x3 << (LED3_num * 2));

	// FLASH LEDs to confirm working
	GPIOC_ODR |= (1u << LED1_num);
	GPIOB_ODR |= (1u << LED2_num);
	GPIOB_ODR |= (1u << LED3_num);
	int j = 200000;
	while(j>0) { j--; }
	GPIOC_ODR &= ~(1u << LED1_num);
	GPIOB_ODR &= ~(1u << LED2_num);
	GPIOB_ODR &= ~(1u << LED3_num);


	// CONFIGURE PUSH BUTTON INPUT
	GPIOC_MODER &= ~(0x3 << (BUTTON_pin*2)); // set to 00: input mode
	GPIOC_OTYPER &= ~(0b1 << BUTTON_pin); // set to 0: reset state
	GPIOC_OSPEEDR &= ~(0b00 << (BUTTON_pin * 2)); // N/A
//
//	GPIOC_PUPDR &= ~(0x3 << (BUTTON_pin * 2)); // clear
//	//GPIOC_PUPDR |= (0b10 << (BUTTON_pin * 2)); // enable pull-up
//	GPIOC_PUPDR |= (0b01 << (BUTTON_pin * 2)); // enable pull-down


	// CONFIGURE push button interrupt
	SYSCFG_EXTICR4 &= ~(0xF << 4); // clear interrupt 13
	SYSCFG_EXTICR4 |= (0x2 << 4); // port C
	EXTI_IMR1 |= (0b1 << BUTTON_pin);   // unmask EXTI13
		//	EXTI_RTSR1 |= (1 << BUTTON_pin);  // enable rising edge (button press)
		//	EXTI_FTSR1 &= ~(1 << BUTTON_pin); // disable falling edge (button release)
	EXTI_RTSR1 &= ~(0b1 << BUTTON_pin);  // disable rising edge (button press)
	EXTI_FTSR1 |= (0b1 << BUTTON_pin); // enable falling edge (button release)
	EXTI_PR1 |= (0b1 << 13); // clear pending flag
	NVIC_ISER1 |= (1u << 8); // the handler for EXTI15-10 is IRQ #40, so reg 2, bit 8 (40-32=8)


	// CONFIGURE TIMER
	RCC_CR |= (0b1 << 8); // enable HSI16
	// while((RCC_CR & (0b1 << 10)) == 0);
	RCC_CFGR &= ~(0b11); // clear System clock switch
	RCC_CFGR |= (0b01); // HSI16 clock selected.
	// while ((RCC_CFGR & (0x3 << 2)) != (0x1 << 2)) { ; /* wait until SWS == 01 (HSI selected)*/ }
	unsigned int prescale = 63999; // 0b1111100111111111;
	unsigned int arr = 249;

	TIM4_PSC = prescale;
	TIM4_ARR = arr;

	TIM4_EGR |= 0x1; // event update
	TIM4_DIER |= 0x1; // Update interrupt enabled
	TIM4_CR1 |= 0x1; // Counter enabled

	NVIC_ISER0 |= (1u << 30); // enable TIM4 interrupt line, IRQ 30 (page 469 in manual)



	unsigned int i = 0;
	while(1) {
		i++;
		if (i > 100000) i = 0;  // turn LED on
	}
}


void EXTI15_10_IRQHandler(void) {
	for (volatile int i = 0; i < 1000; i++); // debounce
    // Toggle the LED state
	LED_State = !LED_State;  // flip 0 <-> 1

	// PART 2:
    // Apply the new state to the LED
//    if (LED_State) {
//        GPIOB_ODR |= (0b1 << LED2_num);  // turn LED on
//    } else {
//        GPIOB_ODR &= ~(0b1 << LED2_num); // turn LED off
//    }

    EXTI_PR1 |= (0b1 << 13);  // Clear the EXTI interrupt pending bit
}

void TIM4_IRQHandler(void) {
	// conditional wrapper only for PART 4
	if (LED_State) {
		if (LED_cycle_state == 0) {
			GPIOC_ODR |= (0b1 << LED1_num);  // turn LED on
			GPIOB_ODR &= ~(0b1 << LED3_num); // turn LED off
			LED_cycle_state++;
		} else if (LED_cycle_state == 1) {
			GPIOB_ODR |= (0b1 << LED2_num);  // turn LED on
			GPIOC_ODR &= ~(0b1 << LED1_num); // turn LED off
			LED_cycle_state++;
		} else if (LED_cycle_state == 2) {
			GPIOB_ODR |= (0b1 << LED3_num);  // turn LED on
			GPIOB_ODR &= ~(0b1 << LED2_num); // turn LED off
			LED_cycle_state = 0;
		}
	}
	TIM4_SR &= ~(0b1);  // Clear the EXTI interrupt pending bit
}

