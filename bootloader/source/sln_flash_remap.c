/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board.h"
#include "sln_flash_remap.h"
#include "sln_flash_config.h"

#if (defined(MIMXRT106A_SERIES) || defined(MIMXRT106L_SERIES) || defined(MIMXRT106F_SERIES) || \
     defined(MIMXRT106S_SERIES) || defined(MIMXRT106P_SERIES))
#define MIMXRT106X_SERIES
#elif (defined(MIMXRT1176_cm7_SERIES) || defined(MIMXRT117F_cm7_SERIES) || defined(MIMXRT117H_cm7_SERIES))
#define MIMXRT117X_SERIES
#else
#error "Remap mechanism is not defined for this device!"
#endif

#if defined(MIMXRT106X_SERIES)
#define FLASH_REMAP_ADDR_START_MASK  (~IOMUXC_GPR_GPR30_FLEXSPI_REMAP_ADDR_START_MASK)
#define FLASH_REMAP_ADDR_END_MASK    (~IOMUXC_GPR_GPR31_FLEXSPI_REMAP_ADDR_END_MASK)
#define FLASH_REMAP_ADDR_OFFSET_MASK (~IOMUXC_GPR_GPR32_FLEXSPI_REMAP_ADDR_OFFSET_MASK)
#elif defined(MIMXRT117X_SERIES)
#define FLASH_REMAP_ADDR_START_MASK  (~FLEXSPI_HADDRSTART_ADDRSTART_MASK)
#define FLASH_REMAP_ADDR_END_MASK    (~FLEXSPI_HADDREND_ENDSTART_MASK)
#define FLASH_REMAP_ADDR_OFFSET_MASK (~FLEXSPI_HADDROFFSET_ADDROFFSET_MASK)
#endif /* MIMXRT106X_SERIES */

void SLN_DisableRemap(void)
{
#if defined(MIMXRT106X_SERIES)
    IOMUXC_GPR->GPR30 = 0;
    IOMUXC_GPR->GPR31 = 0;
    IOMUXC_GPR->GPR32 = 0;
#elif defined(MIMXRT117X_SERIES)
    BOARD_FLEXSPI->HADDRSTART  = FLEXSPI_HADDRSTART_REMAPEN(false);
    BOARD_FLEXSPI->HADDREND    = 0;
    BOARD_FLEXSPI->HADDROFFSET = 0;
#endif /* MIMXRT106X_SERIES */
}

status_t SLN_EnableRemap(uint32_t addrStart, uint32_t addrStop, uint32_t offset)
{
    status_t status = kStatus_Success;

    /* Check alignment */
    if (((addrStart & FLASH_REMAP_ADDR_START_MASK) != 0) || ((addrStop & FLASH_REMAP_ADDR_END_MASK) != 0) ||
        ((offset & FLASH_REMAP_ADDR_OFFSET_MASK) != 0))
    {
        status = kStatus_InvalidArgument;
    }
    else if ((offset == 0) || (offset < (addrStop - addrStart)))
    {
        /* offset shouldn't be 0 */
        status = kStatus_InvalidArgument;
    }
    else if ((addrStop > (FLASH_SIZE + FLEXSPI_AMBA_BASE)) ||
             ((addrStop - 1) + offset) > (FLASH_SIZE + FLEXSPI_AMBA_BASE))
    {
        /* Check that addrStop is larger than FLEXSPI_AMBA_BASE and addrStop + offset is less than flash last size */
        status = kStatus_InvalidArgument;
    }
    else if ((addrStart < FLEXSPI_AMBA_BASE) || (addrStart >= addrStop))
    {
        /* check that addrStart is larger than FLEXSPI_AMBA_BASE and less than addrStop */
        status = kStatus_InvalidArgument;
    }

    if (status == kStatus_Success)
    {
#if defined(MIMXRT106X_SERIES)
        IOMUXC_GPR->GPR30 = addrStart & IOMUXC_GPR_GPR30_FLEXSPI_REMAP_ADDR_START_MASK;
        IOMUXC_GPR->GPR31 = addrStop & IOMUXC_GPR_GPR31_FLEXSPI_REMAP_ADDR_END_MASK;
        IOMUXC_GPR->GPR32 = offset & IOMUXC_GPR_GPR32_FLEXSPI_REMAP_ADDR_OFFSET_MASK;
#elif defined(MIMXRT117X_SERIES)
        BOARD_FLEXSPI->HADDRSTART  = addrStart & FLEXSPI_HADDRSTART_ADDRSTART_MASK;
        BOARD_FLEXSPI->HADDREND    = addrStop & FLEXSPI_HADDREND_ENDSTART_MASK;
        BOARD_FLEXSPI->HADDROFFSET = (offset & FLEXSPI_HADDROFFSET_ADDROFFSET_MASK);
        BOARD_FLEXSPI->HADDRSTART |= FLEXSPI_HADDRSTART_REMAPEN(true);
#endif /* MIMXRT106X_SERIES */
    }

    return status;
}
