/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2022
 * Generated linker script file for MIMXRT1176xxxxx
 * Created from linkscript.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.5.0 [Build 7232] [2022-01-11] on Apr 12, 2022, 11:46:03 PM
 */

INCLUDE "sln_smart_tlhmi_elevator_cm4_Debug_library.ld"
INCLUDE "sln_smart_tlhmi_elevator_cm4_Debug_memory.ld"

ENTRY(ResetISR)

SECTIONS
{
     /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        LONG(LOADADDR(.data_RAM4));
        LONG(    ADDR(.data_RAM4));
        LONG(  SIZEOF(.data_RAM4));
        LONG(LOADADDR(.data_RAM5));
        LONG(    ADDR(.data_RAM5));
        LONG(  SIZEOF(.data_RAM5));
        LONG(LOADADDR(.data_RAM6));
        LONG(    ADDR(.data_RAM6));
        LONG(  SIZEOF(.data_RAM6));
        LONG(LOADADDR(.data_RAM7));
        LONG(    ADDR(.data_RAM7));
        LONG(  SIZEOF(.data_RAM7));
        LONG(LOADADDR(.data_RAM8));
        LONG(    ADDR(.data_RAM8));
        LONG(  SIZEOF(.data_RAM8));
        LONG(LOADADDR(.data_RAM9));
        LONG(    ADDR(.data_RAM9));
        LONG(  SIZEOF(.data_RAM9));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        LONG(    ADDR(.bss_RAM4));
        LONG(  SIZEOF(.bss_RAM4));
        LONG(    ADDR(.bss_RAM5));
        LONG(  SIZEOF(.bss_RAM5));
        LONG(    ADDR(.bss_RAM6));
        LONG(  SIZEOF(.bss_RAM6));
        LONG(    ADDR(.bss_RAM7));
        LONG(  SIZEOF(.bss_RAM7));
        LONG(    ADDR(.bss_RAM8));
        LONG(  SIZEOF(.bss_RAM8));
        LONG(    ADDR(.bss_RAM9));
        LONG(  SIZEOF(.bss_RAM9));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)

       *(.text*)
       KEEP(*freertos*/tasks.o(.rodata*)) /* FreeRTOS Debug Config */
       *(.rodata .rodata.* .constdata .constdata.*)
       . = ALIGN(4);
            /* C++ constructors etc */
            . = ALIGN(4);
            KEEP(*(.init))
            
            . = ALIGN(4);
            __preinit_array_start = .;
            KEEP (*(.preinit_array))
            __preinit_array_end = .;
            
            . = ALIGN(4);
            __init_array_start = .;
            KEEP (*(SORT(.init_array.*)))
            KEEP (*(.init_array))
            __init_array_end = .;
            
            KEEP(*(.fini));
            
            . = ALIGN(4);
            KEEP (*crtbegin.o(.ctors))
            KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
            KEEP (*(SORT(.ctors.*)))
            KEEP (*crtend.o(.ctors))
            
            . = ALIGN(4);
            KEEP (*crtbegin.o(.dtors))
            KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
            KEEP (*(SORT(.dtors.*)))
            KEEP (*crtend.o(.dtors))
            . = ALIGN(4);
            /* End C++ */
    } > BOARD_SDRAM
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > BOARD_SDRAM

    .ARM.exidx : ALIGN(0x20)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . += 24;
        __exidx_end = .;
    } > BOARD_SDRAM
 
    _etext = .;
        
    /* DATA section for SRAM_DTC_cm4 */

    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        PROVIDE(__start_data_SRAM_DTC_cm4 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$SRAM_DTC_cm4)
        *(.data.$RAM2)
        *(.data.$SRAM_DTC_cm4)
        *(.data.$RAM2.*)
        *(.data.$SRAM_DTC_cm4.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
        PROVIDE(__end_data_SRAM_DTC_cm4 = .) ;
     } > SRAM_DTC_cm4 AT>BOARD_SDRAM

    /* DATA section for SRAM_ITC_cm4 */

    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        PROVIDE(__start_data_SRAM_ITC_cm4 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$SRAM_ITC_cm4)
        *(.data.$RAM3)
        *(.data.$SRAM_ITC_cm4)
        *(.data.$RAM3.*)
        *(.data.$SRAM_ITC_cm4.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
        PROVIDE(__end_data_SRAM_ITC_cm4 = .) ;
     } > SRAM_ITC_cm4 AT>BOARD_SDRAM

    /* DATA section for rpmsg_sh_mem */

    .data_RAM4 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        PROVIDE(__start_data_rpmsg_sh_mem = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$rpmsg_sh_mem)
        *(.data.$RAM4)
        *(.data.$rpmsg_sh_mem)
        *(.data.$RAM4.*)
        *(.data.$rpmsg_sh_mem.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM4 = .) ;
        PROVIDE(__end_data_rpmsg_sh_mem = .) ;
     } > rpmsg_sh_mem AT>BOARD_SDRAM

    /* DATA section for fb_sh_mem */

    .data_RAM5 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM5 = .) ;
        PROVIDE(__start_data_fb_sh_mem = .) ;
        *(.ramfunc.$RAM5)
        *(.ramfunc.$fb_sh_mem)
        *(.data.$RAM5)
        *(.data.$fb_sh_mem)
        *(.data.$RAM5.*)
        *(.data.$fb_sh_mem.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM5 = .) ;
        PROVIDE(__end_data_fb_sh_mem = .) ;
     } > fb_sh_mem AT>BOARD_SDRAM

    /* DATA section for NCACHE_REGION */

    .data_RAM6 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM6 = .) ;
        PROVIDE(__start_data_NCACHE_REGION = .) ;
        *(.ramfunc.$RAM6)
        *(.ramfunc.$NCACHE_REGION)
       *(NonCacheable.init)
       *(m_usb_dma_init_data)
        *(.data.$RAM6)
        *(.data.$NCACHE_REGION)
        *(.data.$RAM6.*)
        *(.data.$NCACHE_REGION.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM6 = .) ;
        PROVIDE(__end_data_NCACHE_REGION = .) ;
     } > NCACHE_REGION AT>BOARD_SDRAM

    /* DATA section for BOARD_SDRAM_CM7 */

    .data_RAM7 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM7 = .) ;
        PROVIDE(__start_data_BOARD_SDRAM_CM7 = .) ;
        *(.ramfunc.$RAM7)
        *(.ramfunc.$BOARD_SDRAM_CM7)
        *(.data.$RAM7)
        *(.data.$BOARD_SDRAM_CM7)
        *(.data.$RAM7.*)
        *(.data.$BOARD_SDRAM_CM7.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM7 = .) ;
        PROVIDE(__end_data_BOARD_SDRAM_CM7 = .) ;
     } > BOARD_SDRAM_CM7 AT>BOARD_SDRAM

    /* DATA section for NCACHE_REGION_CM7 */

    .data_RAM8 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM8 = .) ;
        PROVIDE(__start_data_NCACHE_REGION_CM7 = .) ;
        *(.ramfunc.$RAM8)
        *(.ramfunc.$NCACHE_REGION_CM7)
        *(.data.$RAM8)
        *(.data.$NCACHE_REGION_CM7)
        *(.data.$RAM8.*)
        *(.data.$NCACHE_REGION_CM7.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM8 = .) ;
        PROVIDE(__end_data_NCACHE_REGION_CM7 = .) ;
     } > NCACHE_REGION_CM7 AT>BOARD_SDRAM

    /* DATA section for res_sh_mem */

    .data_RAM9 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM9 = .) ;
        PROVIDE(__start_data_res_sh_mem = .) ;
        *(.ramfunc.$RAM9)
        *(.ramfunc.$res_sh_mem)
        *(.data.$RAM9)
        *(.data.$res_sh_mem)
        *(.data.$RAM9.*)
        *(.data.$res_sh_mem.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM9 = .) ;
        PROVIDE(__end_data_res_sh_mem = .) ;
     } > res_sh_mem AT>BOARD_SDRAM

    /* MAIN DATA SECTION */
    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
       . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* Main DATA section (BOARD_SDRAM) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       PROVIDE(__start_data_RAM = .) ;
       PROVIDE(__start_data_BOARD_SDRAM = .) ;
       *(vtable)
       *(.ramfunc*)
       KEEP(*(CodeQuickAccess))
       KEEP(*(DataQuickAccess))
       *(RamFunction)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
       PROVIDE(__end_data_RAM = .) ;
       PROVIDE(__end_data_BOARD_SDRAM = .) ;
    } > BOARD_SDRAM AT>BOARD_SDRAM

    /* BSS section for SRAM_DTC_cm4 */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       PROVIDE(__start_bss_SRAM_DTC_cm4 = .) ;
       *(.dtc_non_cacheable_data)
       *(.bss.$RAM2)
       *(.bss.$SRAM_DTC_cm4)
       *(.bss.$RAM2.*)
       *(.bss.$SRAM_DTC_cm4.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
       PROVIDE(__end_bss_SRAM_DTC_cm4 = .) ;
    } > SRAM_DTC_cm4 AT> SRAM_DTC_cm4

    /* BSS section for SRAM_ITC_cm4 */
    .bss_RAM3 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       PROVIDE(__start_bss_SRAM_ITC_cm4 = .) ;
       *(.bss.$RAM3)
       *(.bss.$SRAM_ITC_cm4)
       *(.bss.$RAM3.*)
       *(.bss.$SRAM_ITC_cm4.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
       PROVIDE(__end_bss_SRAM_ITC_cm4 = .) ;
    } > SRAM_ITC_cm4 AT> SRAM_ITC_cm4

    /* BSS section for rpmsg_sh_mem */
    .bss_RAM4 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       PROVIDE(__start_bss_rpmsg_sh_mem = .) ;
       *(.bss.$RAM4)
       *(.bss.$rpmsg_sh_mem)
       *(.bss.$RAM4.*)
       *(.bss.$rpmsg_sh_mem.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
       PROVIDE(__end_bss_rpmsg_sh_mem = .) ;
    } > rpmsg_sh_mem AT> rpmsg_sh_mem

    /* BSS section for fb_sh_mem */
    .bss_RAM5 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM5 = .) ;
       PROVIDE(__start_bss_fb_sh_mem = .) ;
       *(.bss.$RAM5)
       *(.bss.$fb_sh_mem)
       *(.bss.$RAM5.*)
       *(.bss.$fb_sh_mem.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM5 = .) ;
       PROVIDE(__end_bss_fb_sh_mem = .) ;
    } > fb_sh_mem AT> fb_sh_mem

    /* BSS section for NCACHE_REGION */
    .bss_RAM6 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM6 = .) ;
       PROVIDE(__start_bss_NCACHE_REGION = .) ;
       *(NonCacheable)
       *(m_usb_dma_noninit_data)
       *(.bss.$RAM6)
       *(.bss.$NCACHE_REGION)
       *(.bss.$RAM6.*)
       *(.bss.$NCACHE_REGION.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM6 = .) ;
       PROVIDE(__end_bss_NCACHE_REGION = .) ;
    } > NCACHE_REGION AT> NCACHE_REGION

    /* BSS section for BOARD_SDRAM_CM7 */
    .bss_RAM7 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM7 = .) ;
       PROVIDE(__start_bss_BOARD_SDRAM_CM7 = .) ;
       *(.bss.$RAM7)
       *(.bss.$BOARD_SDRAM_CM7)
       *(.bss.$RAM7.*)
       *(.bss.$BOARD_SDRAM_CM7.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM7 = .) ;
       PROVIDE(__end_bss_BOARD_SDRAM_CM7 = .) ;
    } > BOARD_SDRAM_CM7 AT> BOARD_SDRAM_CM7

    /* BSS section for NCACHE_REGION_CM7 */
    .bss_RAM8 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM8 = .) ;
       PROVIDE(__start_bss_NCACHE_REGION_CM7 = .) ;
       *(.bss.$RAM8)
       *(.bss.$NCACHE_REGION_CM7)
       *(.bss.$RAM8.*)
       *(.bss.$NCACHE_REGION_CM7.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM8 = .) ;
       PROVIDE(__end_bss_NCACHE_REGION_CM7 = .) ;
    } > NCACHE_REGION_CM7 AT> NCACHE_REGION_CM7

    /* BSS section for res_sh_mem */
    .bss_RAM9 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM9 = .) ;
       PROVIDE(__start_bss_res_sh_mem = .) ;
       *(.bss.$RAM9)
       *(.bss.$res_sh_mem)
       *(.bss.$RAM9.*)
       *(.bss.$res_sh_mem.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM9 = .) ;
       PROVIDE(__end_bss_res_sh_mem = .) ;
    } > res_sh_mem AT> res_sh_mem

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        PROVIDE(__start_bss_RAM = .) ;
        PROVIDE(__start_bss_BOARD_SDRAM = .) ;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(__end_bss_RAM = .) ;
        PROVIDE(__end_bss_BOARD_SDRAM = .) ;
        PROVIDE(end = .);
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* NOINIT section for SRAM_DTC_cm4 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM2 = .) ;
       PROVIDE(__start_noinit_SRAM_DTC_cm4 = .) ;
       *(.noinit.$RAM2)
       *(.noinit.$SRAM_DTC_cm4)
       *(.noinit.$RAM2.*)
       *(.noinit.$SRAM_DTC_cm4.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM2 = .) ;
       PROVIDE(__end_noinit_SRAM_DTC_cm4 = .) ;
    } > SRAM_DTC_cm4 AT> SRAM_DTC_cm4

    /* NOINIT section for SRAM_ITC_cm4 */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM3 = .) ;
       PROVIDE(__start_noinit_SRAM_ITC_cm4 = .) ;
       *(.noinit.$RAM3)
       *(.noinit.$SRAM_ITC_cm4)
       *(.noinit.$RAM3.*)
       *(.noinit.$SRAM_ITC_cm4.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM3 = .) ;
       PROVIDE(__end_noinit_SRAM_ITC_cm4 = .) ;
    } > SRAM_ITC_cm4 AT> SRAM_ITC_cm4

    /* NOINIT section for rpmsg_sh_mem */
    .noinit_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM4 = .) ;
       PROVIDE(__start_noinit_rpmsg_sh_mem = .) ;
       *(.noinit.$RAM4)
       *(.noinit.$rpmsg_sh_mem)
       *(.noinit.$RAM4.*)
       *(.noinit.$rpmsg_sh_mem.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM4 = .) ;
       PROVIDE(__end_noinit_rpmsg_sh_mem = .) ;
    } > rpmsg_sh_mem AT> rpmsg_sh_mem

    /* NOINIT section for fb_sh_mem */
    .noinit_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM5 = .) ;
       PROVIDE(__start_noinit_fb_sh_mem = .) ;
       *(.noinit.$RAM5)
       *(.noinit.$fb_sh_mem)
       *(.noinit.$RAM5.*)
       *(.noinit.$fb_sh_mem.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM5 = .) ;
       PROVIDE(__end_noinit_fb_sh_mem = .) ;
    } > fb_sh_mem AT> fb_sh_mem

    /* NOINIT section for NCACHE_REGION */
    .noinit_RAM6 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM6 = .) ;
       PROVIDE(__start_noinit_NCACHE_REGION = .) ;
       *(.noinit.$RAM6)
       *(.noinit.$NCACHE_REGION)
       *(.noinit.$RAM6.*)
       *(.noinit.$NCACHE_REGION.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM6 = .) ;
       PROVIDE(__end_noinit_NCACHE_REGION = .) ;
    } > NCACHE_REGION AT> NCACHE_REGION

    /* NOINIT section for BOARD_SDRAM_CM7 */
    .noinit_RAM7 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM7 = .) ;
       PROVIDE(__start_noinit_BOARD_SDRAM_CM7 = .) ;
       *(.noinit.$RAM7)
       *(.noinit.$BOARD_SDRAM_CM7)
       *(.noinit.$RAM7.*)
       *(.noinit.$BOARD_SDRAM_CM7.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM7 = .) ;
       PROVIDE(__end_noinit_BOARD_SDRAM_CM7 = .) ;
    } > BOARD_SDRAM_CM7 AT> BOARD_SDRAM_CM7

    /* NOINIT section for NCACHE_REGION_CM7 */
    .noinit_RAM8 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM8 = .) ;
       PROVIDE(__start_noinit_NCACHE_REGION_CM7 = .) ;
       *(.noinit.$RAM8)
       *(.noinit.$NCACHE_REGION_CM7)
       *(.noinit.$RAM8.*)
       *(.noinit.$NCACHE_REGION_CM7.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM8 = .) ;
       PROVIDE(__end_noinit_NCACHE_REGION_CM7 = .) ;
    } > NCACHE_REGION_CM7 AT> NCACHE_REGION_CM7

    /* NOINIT section for res_sh_mem */
    .noinit_RAM9 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM9 = .) ;
       PROVIDE(__start_noinit_res_sh_mem = .) ;
       *(.noinit.$RAM9)
       *(.noinit.$res_sh_mem)
       *(.noinit.$RAM9.*)
       *(.noinit.$res_sh_mem.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM9 = .) ;
       PROVIDE(__end_noinit_res_sh_mem = .) ;
    } > res_sh_mem AT> res_sh_mem

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        PROVIDE(__start_noinit_RAM = .) ;
        PROVIDE(__start_noinit_BOARD_SDRAM = .) ;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
       PROVIDE(__end_noinit_RAM = .) ;
       PROVIDE(__end_noinit_BOARD_SDRAM = .) ;        
    } > BOARD_SDRAM AT> BOARD_SDRAM

    /* Reserve and place Heap within memory map */
    _HeapSize = 0x1000;
    .heap :  ALIGN(4)
    {
        _pvHeapStart = .;
        . += _HeapSize;
        . = ALIGN(4);
        _pvHeapLimit = .;
    } > BOARD_SDRAM

     _StackSize = 0x1000;
     /* Reserve space in memory for Stack */
    .heap2stackfill  :
    {
        . += _StackSize;
    } > BOARD_SDRAM
    /* Locate actual Stack in memory map */
    .stack ORIGIN(BOARD_SDRAM) + LENGTH(BOARD_SDRAM) - _StackSize - 0:  ALIGN(4)
    {
        _vStackBase = .;
        . = ALIGN(4);
        _vStackTop = . + _StackSize;
    } > BOARD_SDRAM

    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
    /* Provide symbols for MIMXRT1170 parts for boot header generation code
     * to set image to be plain load image or XIP.
     * Config : Plain load image = false
     */
    _boot_loadaddr = ORIGIN(BOARD_SDRAM);
    _boot_size = LENGTH(BOARD_SDRAM);
}