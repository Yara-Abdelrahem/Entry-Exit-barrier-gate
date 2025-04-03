/* ****************************************************************************
 * File Name          : startup_stm32f401xc.s
 * Author             : MCD Application Team
 * Description        : STM32F401xc devices vector table for GNU ARM toolchain.
 *                      This module performs:
 *                      - Set the initial SP
 *                      - Set the initial PC == Reset_Handler
 *                      - Set the vector table entries with the exceptions ISR address
 *                      - Branches to __main in the C library (which eventually
 *                        calls main()).
 *                      After Reset the CortexM4 processor is in Thread mode,
 *                      priority is Privileged, and the Stack is set to Main.
 ******************************************************************************
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 ******************************************************************************
 */

/* Amount of memory (in bytes) allocated for Stack
   Tailor this value to your application needs */

.thumb
.syntax unified

/* Vector Table Mapped to Address 0 at Reset */
.section .isr_vector, "a", %progbits
.global __Vectors
.global __Vectors_End
.global __Vectors_Size

__Vectors:
    .word __initial_sp              /* Top of Stack */
    .word Reset_Handler             /* Reset Handler */
    .word NMI_Handler               /* NMI Handler */
    .word HardFault_Handler         /* Hard Fault Handler */
    .word MemManage_Handler         /* MPU Fault Handler */
    .word BusFault_Handler          /* Bus Fault Handler */
    .word UsageFault_Handler        /* Usage Fault Handler */
    .word 0                         /* Reserved */
    .word 0                         /* Reserved */
    .word 0                         /* Reserved */
    .word 0                         /* Reserved */
    .word SVC_Handler               /* SVCall Handler */
    .word DebugMon_Handler          /* Debug Monitor Handler */
    .word 0                         /* Reserved */
    .word PendSV_Handler            /* PendSV Handler */
    .word SysTick_Handler           /* SysTick Handler */

    /* External Interrupts */
    .word WWDG_IRQHandler                   /* Window WatchDog */
    .word PVD_IRQHandler                    /* PVD through EXTI Line detection */
    .word TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */
    .word RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */
    .word FLASH_IRQHandler                  /* FLASH */
    .word RCC_IRQHandler                    /* RCC */
    .word EXTI0_IRQHandler                  /* EXTI Line0 */
    .word EXTI1_IRQHandler                  /* EXTI Line1 */
    .word EXTI2_IRQHandler                  /* EXTI Line2 */
    .word EXTI3_IRQHandler                  /* EXTI Line3 */
    .word EXTI4_IRQHandler                  /* EXTI Line4 */
    .word DMA1_Stream0_IRQHandler           /* DMA1 Stream 0 */
    .word DMA1_Stream1_IRQHandler           /* DMA1 Stream 1 */
    .word DMA1_Stream2_IRQHandler           /* DMA1 Stream 2 */
    .word DMA1_Stream3_IRQHandler           /* DMA1 Stream 3 */
    .word DMA1_Stream4_IRQHandler           /* DMA1 Stream 4 */
    .word DMA1_Stream5_IRQHandler           /* DMA1 Stream 5 */
    .word DMA1_Stream6_IRQHandler           /* DMA1 Stream 6 */
    .word ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word EXTI9_5_IRQHandler                /* External Line[9:5]s */
    .word TIM1_BRK_TIM9_IRQHandler          /* TIM1 Break and TIM9 */
    .word TIM1_UP_TIM10_IRQHandler          /* TIM1 Update and TIM10 */
    .word TIM1_TRG_COM_TIM11_IRQHandler     /* TIM1 Trigger and Commutation and TIM11 */
    .word TIM1_CC_IRQHandler                /* TIM1 Capture Compare */
    .word TIM2_IRQHandler                   /* TIM2 */
    .word TIM3_IRQHandler                   /* TIM3 */
    .word TIM4_IRQHandler                   /* TIM4 */
    .word I2C1_EV_IRQHandler                /* I2C1 Event */
    .word I2C1_ER_IRQHandler                /* I2C1 Error */
    .word I2C2_EV_IRQHandler                /* I2C2 Event */
    .word I2C2_ER_IRQHandler                /* I2C2 Error */
    .word SPI1_IRQHandler                   /* SPI1 */
    .word SPI2_IRQHandler                   /* SPI2 */
    .word USART1_IRQHandler                 /* USART1 */
    .word USART2_IRQHandler                 /* USART2 */
    .word 0                                 /* Reserved */
    .word EXTI15_10_IRQHandler              /* External Line[15:10]s */
    .word RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */
    .word OTG_FS_WKUP_IRQHandler            /* USB OTG FS Wakeup through EXTI line */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word DMA1_Stream7_IRQHandler           /* DMA1 Stream7 */
    .word 0                                 /* Reserved */
    .word SDIO_IRQHandler                   /* SDIO */
    .word TIM5_IRQHandler                   /* TIM5 */
    .word SPI3_IRQHandler                   /* SPI3 */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word DMA2_Stream0_IRQHandler           /* DMA2 Stream 0 */
    .word DMA2_Stream1_IRQHandler           /* DMA2 Stream 1 */
    .word DMA2_Stream2_IRQHandler           /* DMA2 Stream 2 */
    .word DMA2_Stream3_IRQHandler           /* DMA2 Stream 3 */
    .word DMA2_Stream4_IRQHandler           /* DMA2 Stream 4 */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word OTG_FS_IRQHandler                 /* USB OTG FS */
    .word DMA2_Stream5_IRQHandler           /* DMA2 Stream 5 */
    .word DMA2_Stream6_IRQHandler           /* DMA2 Stream 6 */
    .word DMA2_Stream7_IRQHandler           /* DMA2 Stream 7 */
    .word USART6_IRQHandler                 /* USART6 */
    .word I2C3_EV_IRQHandler                /* I2C3 event */
    .word I2C3_ER_IRQHandler                /* I2C3 error */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word FPU_IRQHandler                    /* FPU */
    .word 0                                 /* Reserved */
    .word 0                                 /* Reserved */
    .word SPI4_IRQHandler                   /* SPI4 */
__Vectors_End:

.equ __Vectors_Size, __Vectors_End - __Vectors

.section .text
.thumb
.syntax unified


/* Reset handler */
.type Reset_Handler, %function
.global Reset_Handler
.extern main
.extern _sdata
.extern _edata
.extern _sidata
.extern _sbss
.extern _ebss
.extern _heap_start
.extern _heap_end
.extern __initial_sp
Reset_Handler:
    
    ldr sp, =__initial_sp

    
    ldr r0, =_sidata    @ Source in Flash
    ldr r1, =_sdata     @ Destination in RAM
    ldr r2, =_edata     @ End of .data
copy_data:
    cmp r1, r2
    bge zero_bss
    ldr r3, [r0], #4
    str r3, [r1], #4
    b copy_data

  
zero_bss:
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0
clear_bss:
    cmp r1, r2
    bge jump_to_main
    str r3, [r1], #4
    b clear_bss

    
jump_to_main:
    bl main

    
hang:
    b hang

/* Dummy Exception Handlers (infinite loops which can be modified) */
.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b       Infinite_Loop
.size Default_Handler, .-Default_Handler

.macro def_irq_handler handler_name
.weak \handler_name
.set \handler_name, Default_Handler
.endm

/* Exception Handlers */
def_irq_handler NMI_Handler
def_irq_handler HardFault_Handler
def_irq_handler MemManage_Handler
def_irq_handler BusFault_Handler
def_irq_handler UsageFault_Handler
def_irq_handler SVC_Handler
def_irq_handler DebugMon_Handler
def_irq_handler PendSV_Handler
def_irq_handler SysTick_Handler

/* IRQ Handlers */
def_irq_handler WWDG_IRQHandler
def_irq_handler PVD_IRQHandler
def_irq_handler TAMP_STAMP_IRQHandler
def_irq_handler RTC_WKUP_IRQHandler
def_irq_handler FLASH_IRQHandler
def_irq_handler RCC_IRQHandler
def_irq_handler EXTI0_IRQHandler
def_irq_handler EXTI1_IRQHandler
def_irq_handler EXTI2_IRQHandler
def_irq_handler EXTI3_IRQHandler
def_irq_handler EXTI4_IRQHandler
def_irq_handler DMA1_Stream0_IRQHandler
def_irq_handler DMA1_Stream1_IRQHandler
def_irq_handler DMA1_Stream2_IRQHandler
def_irq_handler DMA1_Stream3_IRQHandler
def_irq_handler DMA1_Stream4_IRQHandler
def_irq_handler DMA1_Stream5_IRQHandler
def_irq_handler DMA1_Stream6_IRQHandler
def_irq_handler ADC_IRQHandler
def_irq_handler EXTI9_5_IRQHandler
def_irq_handler TIM1_BRK_TIM9_IRQHandler
def_irq_handler TIM1_UP_TIM10_IRQHandler
def_irq_handler TIM1_TRG_COM_TIM11_IRQHandler
def_irq_handler TIM1_CC_IRQHandler
def_irq_handler TIM2_IRQHandler
def_irq_handler TIM3_IRQHandler
def_irq_handler TIM4_IRQHandler
def_irq_handler I2C1_EV_IRQHandler
def_irq_handler I2C1_ER_IRQHandler
def_irq_handler I2C2_EV_IRQHandler
def_irq_handler I2C2_ER_IRQHandler
def_irq_handler SPI1_IRQHandler
def_irq_handler SPI2_IRQHandler
def_irq_handler USART1_IRQHandler
def_irq_handler USART2_IRQHandler
def_irq_handler EXTI15_10_IRQHandler
def_irq_handler RTC_Alarm_IRQHandler
def_irq_handler OTG_FS_WKUP_IRQHandler
def_irq_handler DMA1_Stream7_IRQHandler
def_irq_handler SDIO_IRQHandler
def_irq_handler TIM5_IRQHandler
def_irq_handler SPI3_IRQHandler
def_irq_handler DMA2_Stream0_IRQHandler
def_irq_handler DMA2_Stream1_IRQHandler
def_irq_handler DMA2_Stream2_IRQHandler
def_irq_handler DMA2_Stream3_IRQHandler
def_irq_handler DMA2_Stream4_IRQHandler
def_irq_handler OTG_FS_IRQHandler
def_irq_handler DMA2_Stream5_IRQHandler
def_irq_handler DMA2_Stream6_IRQHandler
def_irq_handler DMA2_Stream7_IRQHandler
def_irq_handler USART6_IRQHandler
def_irq_handler I2C3_EV_IRQHandler
def_irq_handler I2C3_ER_IRQHandler
def_irq_handler FPU_IRQHandler
def_irq_handler SPI4_IRQHandler

.end
