/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_edma.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */
#include "fsl_iomuxc.h"
#include "fsl_sema4.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
__attribute__((section(".vectorTableRam"), aligned(0x400))) uint32_t g_vectorTable[256] = {0};

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t __Vectors[];
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#define __VECTOR_TABLE __Vectors
#define __StackTop     Image$$ARM_LIB_STACK$$ZI$$Limit
#elif defined(__MCUXPRESSO)
extern uint32_t __Vectors[];
extern void _vStackTop(void);
#define __VECTOR_TABLE __Vectors
#define __StackTop     _vStackTop
#elif defined(__ICCARM__)
extern uint32_t __vector_table[];
extern uint32_t CSTACK$$Limit;
#define __VECTOR_TABLE __vector_table
#define __StackTop     CSTACK$$Limit
#elif defined(__GNUC__)
extern uint32_t __StackTop;
extern uint32_t __Vectors[];
#define __VECTOR_TABLE __Vectors
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get debug console frequency. */
uint32_t BOARD_DebugConsoleSrcFreq(void)
{
#if DEBUG_CONSOLE_UART_INDEX == 1
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart1);
#elif DEBUG_CONSOLE_UART_INDEX == 12
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart12);
#else
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart2);
#endif
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t lpi2cConfig = {0};

    /*
     * lpi2cConfig.debugEnable = false;
     * lpi2cConfig.ignoreAck = false;
     * lpi2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * lpi2cConfig.baudRate_Hz = 100000U;
     * lpi2cConfig.busIdleTimeout_ns = 0;
     * lpi2cConfig.pinLowTimeout_ns = 0;
     * lpi2cConfig.sdaGlitchFilterWidth_ns = 0;
     * lpi2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cConfig);
    LPI2C_MasterInit(base, &lpi2cConfig, clkSrc_Hz);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subAddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize)
{
    status_t status;
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = NULL;
    xfer.dataSize       = 0;

    status = LPI2C_MasterTransferBlocking(base, &xfer);

    if (kStatus_Success == status)
    {
        xfer.subaddressSize = 0;
        xfer.direction      = kLPI2C_Read;
        xfer.data           = rxBuff;
        xfer.dataSize       = rxBuffSize;

        status = LPI2C_MasterTransferBlocking(base, &xfer);
    }

    return status;
}

void BOARD_Accel_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_ACCEL_I2C_BASEADDR, BOARD_ACCEL_I2C_CLOCK_FREQ);
}

status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff)
{
    uint8_t data = (uint8_t)txBuff;

    return BOARD_LPI2C_Send(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, &data, 1);
}

status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}

void BOARD_Codec_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_CODEC_I2C_BASEADDR, BOARD_CODEC_I2C_CLOCK_FREQ);
}

status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

void BOARD_Camera_I2C_Init(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);

    BOARD_LPI2C_Init(BOARD_CAMERA_I2C_BASEADDR, CLOCK_GetRootClockFreq(BOARD_CAMERA_I2C_CLOCK_ROOT));
}

status_t BOARD_Camera_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_SendSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                                txBuffSize);
}

status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_ReceiveSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                                   rxBuffSize);
}

void BOARD_MIPIPanelTouch_I2C_Init(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_SOURCE,
        .div      = BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_ROOT, &lpi2cClockConfig);

    BOARD_LPI2C_Init(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR,
                     CLOCK_GetRootClockFreq(BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_ROOT));
}

status_t BOARD_MIPIPanelTouch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize,
                            (uint8_t *)txBuff, txBuffSize);
}

status_t BOARD_MIPIPanelTouch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}
#endif /* SDK_I2C_BASED_COMPONENT_USED */

/* MPU configuration. */
#if __CORTEX_M == 7

void BOARD_InitEDMA(DMA_Type *base)
{
    static bool dma0Initialized = false;
    edma_config_t edmaConfig    = {0};

    if (base == DMA0)
    {
        if (dma0Initialized == false)
        {
            EDMA_GetDefaultConfig(&edmaConfig);
            edmaConfig.enableDebugMode = true;
            EDMA_Init(base, &edmaConfig);

            dma0Initialized = true;
        }
    }
    else
    {
        EDMA_GetDefaultConfig(&edmaConfig);
        edmaConfig.enableDebugMode = true;
        EDMA_Init(base, &edmaConfig);
    }
}

#if __CORTEX_M == 7
#define MAX_MPU_REGIONS 0xF
#elif __CORTEX_M == 4
#define MAX_MPU_REGIONS 0x7
#endif

static uint8_t BOARD_ConfigMPUNonCached(uint8_t regionStartingNumber, uint32_t regionStartingAddress, uint32_t size)
{
    uint8_t numberOfRegions  = 0;
    uint32_t regionAddress   = regionStartingAddress;
    uint8_t availableRegions = MAX_MPU_REGIONS - regionStartingNumber;
    while ((numberOfRegions < availableRegions) && (size != 0))
    {
        /* Greedy approach to find needed regions. Maybe a best fit algorithm is better here to find
         * the min number of regions needed.
         */
        uint32_t power = 0;
        uint8_t region = 0;

        /* Find the first power of 2 smaller than the wanted size */
        while (size >= (1 << (power + 1)))
        {
            power++;
        }

        if (regionAddress % (1 << power) != 0)
        {
            /* let's find the biggest block that is aligned with the region starting address */
            while (regionAddress % (1 << power) != 0)
            {
                power--;
            }
        }
        if (power < 5)
        {
            break;
        }

        region    = regionStartingNumber + numberOfRegions;
        MPU->RBAR = ARM_MPU_RBAR(region, regionAddress);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, power - 1);
        regionAddress += 1 << power;
        size -= 1 << power;
        numberOfRegions++;
    }

    assert(!size);
    return regionStartingNumber + numberOfRegions;
}

void BOARD_ConfigMPU(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t size          = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                                 0 :
                                 ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_SRAM_OCRAM_NCACHED;
    extern uint32_t __top_SRAM_OCRAM_NCACHED;
    uint32_t ocramNonCacheStart = (uint32_t)(&__base_SRAM_OCRAM_NCACHED);
    uint32_t sizeOcramNonCache  = (uint32_t)(&__top_SRAM_OCRAM_NCACHED) - ocramNonCacheStart;
    (void)sizeOcramNonCache;
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t sizeNonCache  = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
    (void)sizeNonCache;
#if defined(__USE_SHMEM)
    extern uint32_t __base_rpmsg_sh_mem;
    extern uint32_t __top_rpmsg_sh_mem;
    uint32_t rpmsgShMemStart = (uint32_t)(&__base_rpmsg_sh_mem);
    uint32_t sizeRpmsgShMem  = (uint32_t)(&__top_rpmsg_sh_mem) - rpmsgShMemStart;
    (void)sizeRpmsgShMem;
#endif
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t size          = (uint32_t)__NCACHE_REGION_SIZE;
#endif

#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }
#endif

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in mpu_armv7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * mpu_armv7.h.
     */

    /*
     * Add default region to deny access to whole address space to workaround speculative prefetch.
     * Refer to Arm errata 1013783-B for more details.
     *
     */
    /* Region 0 setting: Instruction access disabled, No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_NONE, 0, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 2 setting: Memory with Device type, not shareable,  non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x40000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_32MB);

    /* Region 4 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x42000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);
    // MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);

#if defined(CACHE_MODE_WRITE_THROUGH) && CACHE_MODE_WRITE_THROUGH
    /* Region 7 setting: Memory with Normal type, not shareable, write through */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_2MB);
#else
    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_2MB);
#endif /* CACHE_MODE_WRITE_THROUGH */

    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner Non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(8, ocramNonCacheStart);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_256KB);

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x30000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#endif

#ifdef USE_SDRAM
#if defined(CACHE_MODE_WRITE_THROUGH) && CACHE_MODE_WRITE_THROUGH
    /* Region 9 setting: Memory with Normal type, not shareable, write through */
    MPU->RBAR = ARM_MPU_RBAR(10, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_32MB);
#else
    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(10, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#endif /* CACHE_MODE_WRITE_THROUGH */
#endif /* USE_SDRAM */

    /* Keep it last for now */
    /* Region 12 setting: Memory with Normal type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(11, rpmsgShMemStart);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 0, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    uint8_t region = BOARD_ConfigMPUNonCached(12, nonCacheStart, sizeNonCache);
    /* Store it to allocated next region */
    (void) region;

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_EnableDCache();
#endif
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    SCB_EnableICache();
#endif
}
#elif __CORTEX_M == 4
void BOARD_ConfigMPU(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t nonCacheSize  = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                                 0 :
                                 ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t nonCacheSize  = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t nonCacheSize  = (uint32_t)__NCACHE_REGION_SIZE;
#endif
#if defined(__USE_SHMEM)
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RPMSG_SH_MEM$$Base[];
    /* RPMSG_SH_MEM_unused is a auxiliary region which is used to get the whole size of RPMSG_SH_MEM section */
    extern uint32_t Image$$RPMSG_SH_MEM_unused$$Base[];
    extern uint32_t Image$$RPMSG_SH_MEM_unused$$ZI$$Limit[];
    uint32_t rpmsgShmemStart = (uint32_t)Image$$RPMSG_SH_MEM$$Base;
    uint32_t rpmsgShmemSize  = (uint32_t)Image$$RPMSG_SH_MEM_unused$$ZI$$Limit - rpmsgShmemStart;
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_rpmsg_sh_mem;
    extern uint32_t __top_rpmsg_sh_mem;
    uint32_t rpmsgShmemStart = (uint32_t)(&__base_rpmsg_sh_mem);
    uint32_t rpmsgShmemSize  = (uint32_t)(&__top_rpmsg_sh_mem) - rpmsgShmemStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __RPMSG_SH_MEM_START[];
    extern uint32_t __RPMSG_SH_MEM_SIZE[];
    uint32_t rpmsgShmemStart = (uint32_t)__RPMSG_SH_MEM_START;
    uint32_t rpmsgShmemSize  = (uint32_t)__RPMSG_SH_MEM_SIZE;
#endif
#endif
    uint32_t i = 0;

    /* Only config non-cacheable region on system bus */
    assert(nonCacheStart >= 0x20000000);

    /* Disable code bus cache */
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        /* Enable the processor code bus to push all modified lines. */
        LMEM->PCCCR |= LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK | LMEM_PCCCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((LMEM->PCCCR & LMEM_PCCCR_GO_MASK) != 0U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        LMEM->PCCCR &= ~(LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK);
        /* Now disable the cache. */
        LMEM->PCCCR &= ~LMEM_PCCCR_ENCACHE_MASK;
    }

    /* Disable system bus cache */
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        /* Enable the processor system bus to push all modified lines. */
        LMEM->PSCCR |= LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK | LMEM_PSCCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((LMEM->PSCCR & LMEM_PSCCR_GO_MASK) != 0U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        LMEM->PSCCR &= ~(LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK);
        /* Now disable the cache. */
        LMEM->PSCCR &= ~LMEM_PSCCR_ENCACHE_MASK;
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* Region 0 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);

    while ((nonCacheSize >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(nonCacheStart % nonCacheSize));
        assert(nonCacheSize == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 1 setting: Memory with normal type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(1, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
    }

#if defined(__USE_SHMEM)
    i = 0;

    while ((rpmsgShmemSize >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(rpmsgShmemStart % rpmsgShmemSize));
        assert(rpmsgShmemSize == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 2 setting: Memory with device type, shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(2, rpmsgShmemStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 0, 1, 0, i - 1);
    }
#endif

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enables the processor system bus to invalidate all lines in both ways.
    and Initiate the processor system bus cache command. */
    LMEM->PSCCR |= LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK | LMEM_PSCCR_GO_MASK;
    /* Wait until the cache command completes */
    while ((LMEM->PSCCR & LMEM_PSCCR_GO_MASK) != 0U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    LMEM->PSCCR &= ~(LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK);
    /* Now enable the system bus cache. */
    LMEM->PSCCR |= LMEM_PSCCR_ENCACHE_MASK;

    /* Enables the processor code bus to invalidate all lines in both ways.
    and Initiate the processor code bus code cache command. */
    LMEM->PCCCR |= LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_GO_MASK;
    /* Wait until the cache command completes. */
    while ((LMEM->PCCCR & LMEM_PCCCR_GO_MASK) != 0U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    LMEM->PCCCR &= ~(LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK);
    /* Now enable the code bus cache. */
    LMEM->PCCCR |= LMEM_PCCCR_ENCACHE_MASK;
}
#endif

void BOARD_RelocateVectorTableToRam(void)
{
    uint32_t n;
    uint32_t irqMaskValue;

    irqMaskValue = DisableGlobalIRQ();

    SCB_DisableDCache();
    SCB_DisableICache();

    /* Copy the vector table from ROM to RAM */
    for (n = 0; n < ((uint32_t)0x400) / sizeof(uint32_t); n++)
    {
        g_vectorTable[n] = __VECTOR_TABLE[n];
    }

    /* Set application defined stack pointer */
    volatile unsigned int vStackTop = (unsigned int)&__StackTop;
    g_vectorTable[0]                = vStackTop;

    /* Point the VTOR to the position of vector table */
    SCB->VTOR = (uint32_t)g_vectorTable;
    __DSB();

    SCB_EnableICache();
    SCB_EnableDCache();

    EnableGlobalIRQ(irqMaskValue);
}

void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength)
{
}

void BOARD_MMC_Pin_Config(uint32_t speed, uint32_t strength)
{
}

#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"

static IRQn_Type const s_lpi2cIrqs[] = LPI2C_IRQS;
static lpi2c_rtos_handle_t s_masterRTOSHandle[sizeof(s_lpi2cIrqs) / sizeof(s_lpi2cIrqs[0])];

void BOARD_I2C_HWLock(LPI2C_Type *base)
{
    if (base == LPI2C5)
    {
        SEMA4_Lock(BOARD_SEM4_BASE, BOARD_SEM4_I2C5_GATE, BOARD_SEM4_PROC_NUM);
    }
    else if (base == LPI2C6)
    {
        SEMA4_Lock(BOARD_SEM4_BASE, BOARD_SEM4_I2C6_GATE, BOARD_SEM4_PROC_NUM);
    }
}

void BOARD_I2C_HWUnlock(LPI2C_Type *base)
{
    if (base == LPI2C5)
    {
        SEMA4_Unlock(BOARD_SEM4_BASE, BOARD_SEM4_I2C5_GATE);
    }
    else if (base == LPI2C6)
    {
        SEMA4_Unlock(BOARD_SEM4_BASE, BOARD_SEM4_I2C6_GATE);
    }
}

void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    uint32_t instance                  = LPI2C_GetInstance(base);
    lpi2c_master_config_t masterConfig = {0};

    BOARD_I2C_HWLock(base);
    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    LPI2C_RTOS_Init(&s_masterRTOSHandle[instance], base, &masterConfig, clkSrc_Hz);
#else
    LPI2C_MasterInit(base, &masterConfig, clkSrc_Hz);
#endif
    NVIC_SetPriority(s_lpi2cIrqs[instance], configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1);
    BOARD_I2C_HWUnlock(base);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    status_t status;
    lpi2c_master_transfer_t xfer;
    uint32_t instance = LPI2C_GetInstance(base);

    BOARD_I2C_HWLock(base);

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    status = LPI2C_RTOS_Transfer(&s_masterRTOSHandle[instance], &xfer);
#else
    status = LPI2C_MasterTransferBlocking(base, &xfer);
#endif

    BOARD_I2C_HWUnlock(base);

    return status;
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize)
{
    status_t status;
    lpi2c_master_transfer_t xfer;
    uint32_t instance = LPI2C_GetInstance(base);
    BOARD_I2C_HWLock(base);

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    status = LPI2C_RTOS_Transfer(&s_masterRTOSHandle[instance], &xfer);
#else
    status = LPI2C_MasterTransferBlocking(base, &xfer);
#endif

    BOARD_I2C_HWUnlock(base);
    return status;
}

void BOARD_FlexioCameraPullPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_FLEXIO_CAMERA_PWDN_GPIO, BOARD_FLEXIO_CAMERA_PWDN_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_FLEXIO_CAMERA_PWDN_GPIO, BOARD_FLEXIO_CAMERA_PWDN_PIN, 0U);
    }
}

void BOARD_FlexioCameraPullResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_FLEXIO_CAMERA_RESET_GPIO, BOARD_FLEXIO_CAMERA_RESET_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_FLEXIO_CAMERA_RESET_GPIO, BOARD_FLEXIO_CAMERA_RESET_PIN, 0U);
    }
}

void BOARD_FlexioCameraI2CInit(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_FLEXIO_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_FLEXIO_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_FLEXIO_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);

    BOARD_LPI2C_Init(BOARD_FLEXIO_CAMERA_I2C_BASE, CLOCK_GetRootClockFreq(BOARD_FLEXIO_CAMERA_I2C_CLOCK_ROOT));
}

status_t BOARD_FlexioCameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_FLEXIO_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_FlexioCameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_FLEXIO_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}

void BOARD_FlexioCameraConfigDMA()
{
    // DMA0 read qos
    uint32_t *NIC301_ReadQosReg = (uint32_t *)(0x41400000 + 0x47000 + 0x100);
    *NIC301_ReadQosReg          = 0xf;

    // DMA0 write qos
    uint32_t *NIC301_WriteQosReg = (uint32_t *)(0x41400000 + 0x47000 + 0x104);
    *NIC301_WriteQosReg          = 0xf;
}

void BOARD_ShareDb_Lock()
{
    SEMA4_Lock(BOARD_SEM4_BASE, BOARD_SEM4_SHAREDB_GATE, BOARD_SEM4_PROC_NUM);
}

void BOARD_ShareDb_Unlock()
{
    SEMA4_Unlock(BOARD_SEM4_BASE, BOARD_SEM4_SHAREDB_GATE);
}
