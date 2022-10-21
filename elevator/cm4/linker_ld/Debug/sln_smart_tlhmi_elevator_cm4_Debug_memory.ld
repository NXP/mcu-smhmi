/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2022
 * Generated linker script file for MIMXRT1176xxxxx
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.5.0 [Build 7232] [2022-01-11] on Apr 12, 2022, 11:46:03 PM
 */

MEMORY
{
  /* Define each memory region */
  BOARD_SDRAM (rwx) : ORIGIN = 0x80000000, LENGTH = 0xb00000 /* 11M bytes (alias RAM) */  
  SRAM_DTC_cm4 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x20000 /* 128K bytes (alias RAM2) */  
  SRAM_ITC_cm4 (rwx) : ORIGIN = 0x1ffe0000, LENGTH = 0x20000 /* 128K bytes (alias RAM3) */  
  rpmsg_sh_mem (rwx) : ORIGIN = 0x20360000, LENGTH = 0x20000 /* 128K bytes (alias RAM4) */  
  fb_sh_mem (rwx) : ORIGIN = 0x81c00000, LENGTH = 0x200000 /* 2M bytes (alias RAM5) */  
  NCACHE_REGION (rwx) : ORIGIN = 0x81800000, LENGTH = 0x400000 /* 4M bytes (alias RAM6) */  
  BOARD_SDRAM_CM7 (rwx) : ORIGIN = 0x81200000, LENGTH = 0x600000 /* 6M bytes (alias RAM7) */  
  NCACHE_REGION_CM7 (rwx) : ORIGIN = 0x81e00000, LENGTH = 0x200000 /* 2M bytes (alias RAM8) */  
  res_sh_mem (rwx) : ORIGIN = 0x80b00000, LENGTH = 0x700000 /* 7M bytes (alias RAM9) */  
}

  /* Define a symbol for the top of each memory region */
  __base_BOARD_SDRAM = 0x80000000  ; /* BOARD_SDRAM */  
  __base_RAM = 0x80000000 ; /* RAM */  
  __top_BOARD_SDRAM = 0x80000000 + 0xb00000 ; /* 11M bytes */  
  __top_RAM = 0x80000000 + 0xb00000 ; /* 11M bytes */  
  __base_SRAM_DTC_cm4 = 0x20000000  ; /* SRAM_DTC_cm4 */  
  __base_RAM2 = 0x20000000 ; /* RAM2 */  
  __top_SRAM_DTC_cm4 = 0x20000000 + 0x20000 ; /* 128K bytes */  
  __top_RAM2 = 0x20000000 + 0x20000 ; /* 128K bytes */  
  __base_SRAM_ITC_cm4 = 0x1ffe0000  ; /* SRAM_ITC_cm4 */  
  __base_RAM3 = 0x1ffe0000 ; /* RAM3 */  
  __top_SRAM_ITC_cm4 = 0x1ffe0000 + 0x20000 ; /* 128K bytes */  
  __top_RAM3 = 0x1ffe0000 + 0x20000 ; /* 128K bytes */  
  __base_rpmsg_sh_mem = 0x20360000  ; /* rpmsg_sh_mem */  
  __base_RAM4 = 0x20360000 ; /* RAM4 */  
  __top_rpmsg_sh_mem = 0x20360000 + 0x20000 ; /* 128K bytes */  
  __top_RAM4 = 0x20360000 + 0x20000 ; /* 128K bytes */  
  __base_fb_sh_mem = 0x81c00000  ; /* fb_sh_mem */  
  __base_RAM5 = 0x81c00000 ; /* RAM5 */  
  __top_fb_sh_mem = 0x81c00000 + 0x200000 ; /* 2M bytes */  
  __top_RAM5 = 0x81c00000 + 0x200000 ; /* 2M bytes */  
  __base_NCACHE_REGION = 0x81800000  ; /* NCACHE_REGION */  
  __base_RAM6 = 0x81800000 ; /* RAM6 */  
  __top_NCACHE_REGION = 0x81800000 + 0x400000 ; /* 4M bytes */  
  __top_RAM6 = 0x81800000 + 0x400000 ; /* 4M bytes */  
  __base_BOARD_SDRAM_CM7 = 0x81200000  ; /* BOARD_SDRAM_CM7 */  
  __base_RAM7 = 0x81200000 ; /* RAM7 */  
  __top_BOARD_SDRAM_CM7 = 0x81200000 + 0x600000 ; /* 6M bytes */  
  __top_RAM7 = 0x81200000 + 0x600000 ; /* 6M bytes */  
  __base_NCACHE_REGION_CM7 = 0x81e00000  ; /* NCACHE_REGION_CM7 */  
  __base_RAM8 = 0x81e00000 ; /* RAM8 */  
  __top_NCACHE_REGION_CM7 = 0x81e00000 + 0x200000 ; /* 2M bytes */  
  __top_RAM8 = 0x81e00000 + 0x200000 ; /* 2M bytes */  
  __base_res_sh_mem = 0x80b00000  ; /* res_sh_mem */  
  __base_RAM9 = 0x80b00000 ; /* RAM9 */  
  __top_res_sh_mem = 0x80b00000 + 0x700000 ; /* 7M bytes */  
  __top_RAM9 = 0x80b00000 + 0x700000 ; /* 7M bytes */  
