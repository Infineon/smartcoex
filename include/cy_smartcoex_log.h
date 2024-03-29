/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

#ifndef INCLUDED_CY_SMARTCOEX_LOG_H_
#define INCLUDED_CY_SMARTCOEX_LOG_H_

#include "cy_log.h"

/**
 * \defgroup group_smartcoex_macros Macros
 */

/**
 * \addtogroup group_smartcoex_macros
 * \{
 */

/**
 * ENABLE_SMARTCOEX_LOGS
 *
 * When defined, enables debug logs in the Smart Coex library.
 *
 * The application must add this flag to its 'DEFINES' and call
 * `cy_log_init()`. For more details, see README.md.
 */
#ifdef ENABLE_SMARTCOEX_LOGS
#define cy_smartcoex_log_msg cy_log_msg
#else
#define cy_smartcoex_log_msg(a,b,c,...)
#endif

/** \} group_smartcoex_macros */

#endif /* INCLUDED_CY_SMARTCOEX_LOG_H_ */
