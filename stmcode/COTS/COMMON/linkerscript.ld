/* Linker script for STM32F401CC */

/* Memory definition */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Entry point */
ENTRY(Reset_Handler)

/* Sections */
SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))   /* Vector table */
        *(.text*)              /* Program code */
        *(.rodata*)            /* Read-only data */
        . = ALIGN(4);
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH

    .data :
    {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT> FLASH

    _sidata = LOADADDR(.data);

    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    .heap :
    {
        _heap_start = .;
        . += 0x00000200; /* 512 Bytes heap */
        _heap_end = .;
    } > RAM

    .stack :
    {
        . = ALIGN(8);
        _stack_top = ORIGIN(RAM) + LENGTH(RAM);
    } > RAM

    PROVIDE(__heap_base = _heap_start);
    PROVIDE(__heap_limit = _heap_end);
    PROVIDE(__initial_sp = _stack_top);
}