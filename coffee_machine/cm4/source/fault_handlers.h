/*
 * Copyright 2018, 2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file fault_handlers.h
 * @brief This file handles saving and printing the fault logs
 */

#ifndef _FAULT_HANDLERS_H_
#define _FAULT_HANDLERS_H_

#include <stdint.h>

#include "fsl_common.h"

#define FAULT_STATUSREG_LOG_FILE_NAME "fault_statusreg_log.dat"
#define MAX_FLASH_LOGS_SIZE           (MAX_LOG_HISTORY * configLOGGING_MAX_MESSAGE_LENGTH)

/**
 * @brief This are the fault return types
 */
typedef enum _fault_ret
{
    kFaultRet_Error    = -1,
    kFaultRet_NotFound = 0,
    kFaultRet_Found    = 1
} fault_ret_t;

/**
 * @brief This are the faults type
 */
enum _fault_type
{
    kFaultType_None      = 0,
    kFaultType_Hard      = 1,
    kFaultType_MemManage = 2,
    kFaultType_Bus       = 3,
    kFaultType_Usage     = 4
};

/**
 * @brief Processor registers
 */
typedef struct stack_registers
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;  /**< Link register. */
    uint32_t pc;  /**< Program counter. */
    uint32_t psr; /**< Program status register. */
} stack_registers_t;

/**
 * @brief Fault registers and status
 */
typedef struct fault_status
{
    uint32_t fault_type;
    uint32_t hfsr;         /**< HardFault Status Register (SCB->HFSR) */
    uint32_t cfsr;         /**< Configurable Fault Status Register (SCB->CFSR) */
    uint32_t mmfar;        /**< MemManage Fault Address Register (SCB->MMFAR) */
    uint32_t bfar;         /**< BusFault Address Register (SCB->BFAR) */
    stack_registers_t msr; /**< Main Stack Registers*/
    stack_registers_t psr; /**< Process Stack Registers */
} fault_status_t;

#endif /* _FAULT_HANDLERS_H_ */
