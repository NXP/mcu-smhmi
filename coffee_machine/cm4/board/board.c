/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static IRQn_Type const s_lpi2cIrqs[] = LPI2C_IRQS;
static lpi2c_rtos_handle_t s_masterRTOSHandle[sizeof(s_lpi2cIrqs) / sizeof(s_lpi2cIrqs[0])];

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

void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    uint32_t instance                  = LPI2C_GetInstance(base);
    lpi2c_master_config_t masterConfig = {0};

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

    masterConfig.baudRate_Hz = 400000U;

#if defined(SDK_I2C_FREERTOS) && SDK_I2C_FREERTOS
    LPI2C_RTOS_Init(&s_masterRTOSHandle[instance], base, &masterConfig, clkSrc_Hz);
#else
    LPI2C_MasterInit(base, &masterConfig, clkSrc_Hz);
#endif
    NVIC_SetPriority(s_lpi2cIrqs[instance], configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    status_t status;
    uint32_t instance = LPI2C_GetInstance(base);
    lpi2c_master_transfer_t xfer;

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
    uint32_t instance = LPI2C_GetInstance(base);
    lpi2c_master_transfer_t xfer;

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

    return status;
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

status_t BOARD_MIPICameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_MIPI_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_MIPICameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_MIPI_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}

void BOARD_MIPICameraI2CInit(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_MIPI_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_MIPI_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_MIPI_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);
    BOARD_LPI2C_Init(BOARD_MIPI_CAMERA_I2C_BASE, CLOCK_GetRootClockFreq(BOARD_MIPI_CAMERA_I2C_CLOCK_ROOT));
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

void BOARD_BacklightControl(bool on)
{
    if (on)
    {
        GPIO_PinWrite(BOARD_MIPI_PANEL_BL_GPIO, BOARD_MIPI_PANEL_BL_PIN, 0U);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_PANEL_BL_GPIO, BOARD_MIPI_PANEL_BL_PIN, 1U);
    }
}

void BOARD_MIPICameraPullResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_CAMERA_RESET_GPIO, BOARD_MIPI_CAMERA_RESET_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_CAMERA_RESET_GPIO, BOARD_MIPI_CAMERA_RESET_PIN, 0);
    }
}

void BOARD_MIPICameraPullPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_CAMERA_PWDN_GPIO, BOARD_MIPI_CAMERA_PWDN_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_CAMERA_PWDN_GPIO, BOARD_MIPI_CAMERA_PWDN_PIN, 0);
    }
}

void BOARD_CSICameraPullResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_CSI_CAMERA_RESET_GPIO, BOARD_CSI_CAMERA_RESET_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_CSI_CAMERA_RESET_GPIO, BOARD_CSI_CAMERA_RESET_PIN, 0U);
    }
}

void BOARD_CSICameraPullPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_CSI_CAMERA_PWDN_GPIO, BOARD_CSI_CAMERA_PWDN_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_CSI_CAMERA_PWDN_GPIO, BOARD_CSI_CAMERA_PWDN_PIN, 0U);
    }
}

void BOARD_CSICameraI2CInit(void)
{
    const clock_root_config_t lpi2cClockConfig = {
        .clockOff = false,
        .mux      = BOARD_CSI_CAMERA_I2C_CLOCK_SOURCE,
        .div      = BOARD_CSI_CAMERA_I2C_CLOCK_DIVIDER,
    };

    CLOCK_SetRootClock(BOARD_CSI_CAMERA_I2C_CLOCK_ROOT, &lpi2cClockConfig);
    BOARD_LPI2C_Init(BOARD_CSI_CAMERA_I2C_BASE, CLOCK_GetRootClockFreq(BOARD_CSI_CAMERA_I2C_CLOCK_ROOT));
}

status_t BOARD_CSICameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CSI_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_CSICameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CSI_CAMERA_I2C_BASE, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
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

void BOARD_InitEDMA()
{
    edma_config_t edmaConfig;
    DMAMUX_Init(DMAMUX1);
    EDMA_GetDefaultConfig(&edmaConfig);
    edmaConfig.enableDebugMode = true;
    EDMA_Init(DMA1, &edmaConfig);
}

/* MPU configuration. */
#if __CORTEX_M == 7
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

    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t sizeNonCache  = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;

#if defined(__USE_SHMEM)
    extern uint32_t __base_rpmsg_sh_mem;
    extern uint32_t __top_rpmsg_sh_mem;
    uint32_t rpmsgShMemStart = (uint32_t)(&__base_rpmsg_sh_mem);
    uint32_t sizeRpmsgShMem  = (uint32_t)(&__top_rpmsg_sh_mem) - rpmsgShMemStart;

#endif
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t size          = (uint32_t)__NCACHE_REGION_SIZE;
#endif
    volatile uint32_t i = 0;

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

    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_2MB);

    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner Non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(8, ocramNonCacheStart);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_128KB);

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x30000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#endif

    /* Region 10 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(10, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);

    /* TODO MAke it dynamic */
    /* Region 11 setting: Memory with Normal type, not shareable, outer/inner Non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(11, nonCacheStart);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);

    /* Keep it last for now */
    /* Region 12 setting: Memory with Normal type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(12, rpmsgShMemStart);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 0, 1, 0, ARM_MPU_REGION_SIZE_128KB);

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
    uint32_t nonCacheSize = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
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
    uint32_t rpmsgShmemSize = (uint32_t)Image$$RPMSG_SH_MEM_unused$$ZI$$Limit - rpmsgShmemStart;
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_rpmsg_sh_mem;
    extern uint32_t __top_rpmsg_sh_mem;
    extern uint32_t __base_NCACHE_REGION_CM7;
    extern uint32_t __top_NCACHE_REGION_CM7;

    uint32_t rpmsgShmemStart  = (uint32_t)(&__base_rpmsg_sh_mem);
    uint32_t rpmsgShmemSize   = (uint32_t)(&__top_rpmsg_sh_mem) - rpmsgShmemStart;
    uint32_t nonCacheStartCM7 = (uint32_t)(&__base_NCACHE_REGION_CM7);
    uint32_t nonCacheSizeCM7  = (uint32_t)(&__top_NCACHE_REGION_CM7) - nonCacheStartCM7;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __RPMSG_SH_MEM_START[];
    extern uint32_t __RPMSG_SH_MEM_SIZE[];
    uint32_t rpmsgShmemStart = (uint32_t)__RPMSG_SH_MEM_START;
    uint32_t rpmsgShmemSize  = (uint32_t)__RPMSG_SH_MEM_SIZE;
#endif

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$FB_SH_MEM$$Base[];
    /* FB_SH_MEM_unused is a auxiliary region which is used to get the whole size of FB_SH_MEM section */
    extern uint32_t Image$$FB_SH_MEM_unused$$Base[];
    extern uint32_t Image$$FB_SH_MEM_unused$$ZI$$Limit[];
    uint32_t fbShmemStart = (uint32_t)Image$$FB_SH_MEM$$Base;
    uint32_t fbShmemSize = (uint32_t)Image$$FB_SH_MEM_unused$$ZI$$Limit - fbShmemStart;
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_fb_sh_mem;
    extern uint32_t __top_fb_sh_mem;
    uint32_t fbShmemStart = (uint32_t)(&__base_fb_sh_mem);
    uint32_t fbShmemSize  = (uint32_t)(&__top_fb_sh_mem) - fbShmemStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __FB_SH_MEM_START[];
    extern uint32_t __FB_SH_MEM_SIZE[];
    uint32_t fbShmemStart = (uint32_t)__FB_SH_MEM_START;
    uint32_t fbShmemSize  = (uint32_t)__FB_SH_MEM_SIZE;
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

        /* Region 0 setting: Memory device type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(0, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, i - 1);
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
        MPU->RBAR = ARM_MPU_RBAR(1, rpmsgShmemStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, i - 1);
    }

    /* fb_sh_mem */
    i = 0;

    while ((fbShmemSize >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(fbShmemStart % fbShmemSize));
        assert(fbShmemSize == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 2 setting: Memory with device type, shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(2, fbShmemStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, i - 1);
    }

#endif
    /* NCACHE_REGION_CM7 */
    i = 0;

    while ((nonCacheSizeCM7 >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(nonCacheStartCM7 % nonCacheSizeCM7));
        assert(nonCacheSizeCM7 == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 3 setting: Memory with Normal type, not shareable, outer/inner Non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(3, nonCacheStartCM7);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
    }

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

void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength)
{
}

void BOARD_MMC_Pin_Config(uint32_t speed, uint32_t strength)
{
}

__attribute__((weak)) void INPUT_DEV_PUSH_BUTTONS_IRQHandler(GPIO_Type *base, uint32_t intPin)
{
}

void GPIO2_Combined_16_31_IRQHandler(void)
{
    uint32_t intPin        = 0x00;
    uint32_t pushButtonInt = 0x00;
    // Get interrupt flag for the GPIO
    intPin = GPIO_PortGetInterruptFlags(GPIO2);

    pushButtonInt = (1 << BOARD_BUTTON_SW2_PIN) | (1 << BOARD_BUTTON_SW3_PIN);

    // Check for the interrupt pin on the GPIO for USER_BUTTON (SW7)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;

        // Check if interrupt was triggered by a push button
        if (intPin & pushButtonInt)
        {
            INPUT_DEV_PUSH_BUTTONS_IRQHandler(GPIO2, intPin);
            /* clear the interrupt status */
            GPIO_PortClearInterruptFlags(GPIO2, pushButtonInt);

            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
        else
        {
            // TODO
            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
    }
}

void GPIO2_Combined_0_15_IRQHandler(void)
{
    uint32_t intPin        = 0x00;
    uint32_t pushButtonInt = 0x00;
    // Get interrupt flag for the GPIO
    intPin = GPIO_PortGetInterruptFlags(GPIO2);

    pushButtonInt = (1 << BOARD_BUTTON_SW1_PIN);

    // Check for the interrupt pin on the GPIO for USER_BUTTON (SW7)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;

        // Check if interrupt was triggered by a push button
        if (intPin & pushButtonInt)
        {
            INPUT_DEV_PUSH_BUTTONS_IRQHandler(GPIO2, intPin);

            /* clear the interrupt status */
            GPIO_PortClearInterruptFlags(GPIO2, pushButtonInt);

            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
        else
        {
            // TODO
            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
    }
}

void GPIO13_Combined_0_31_IRQHandler(void)
{
    uint32_t intPin        = 0x00;
    uint32_t pushButtonInt = 0x00;
    // Get interrupt flag for the GPIO
    intPin = GPIO_PortGetInterruptFlags(GPIO13);

    pushButtonInt = (1 << BOARD_USER_BUTTON_GPIO_PIN);

    // Check for the interrupt pin on the GPIO for USER_BUTTON (SW7)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;

        // Check if interrupt was triggered by a push button
        if (intPin & pushButtonInt)
        {
            INPUT_DEV_PUSH_BUTTONS_IRQHandler(GPIO13, intPin);
            /* clear the interrupt status */
            GPIO_PortClearInterruptFlags(GPIO13, pushButtonInt);

            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
        else
        {
            // TODO
            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
    }
}
