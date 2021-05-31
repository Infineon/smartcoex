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

/**
* @file cy_smartcoex.h
* @brief Smart Coex is a library that provides functions that are
* used for setting up the coex configuration.
*/

#ifndef INCLUDED_CY_SMARTCOEX_INTERFACE_H_
#define INCLUDED_CY_SMARTCOEX_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cy_result.h"
#include "wiced_bt_dev.h"


/**
 * \defgroup group_smartcoex_enums Enumerated Types
 * \defgroup group_smartcoex_typedefs Typedefs
 * \defgroup group_smartcoex_structs Structures
 * \defgroup group_smartcoex_functions Functions
 */

/******************************************************
 *                   Typedefs
 ******************************************************/

/**
 * \addtogroup group_smartcoex_typedefs
 * \{
 */

/**
 * Callback provided to inform the status of the BT coex setting
 */
typedef wiced_bt_dev_vendor_specific_command_complete_cback_t* btcoex_cb_t;

/** \} group_smartcoex_typedefs */

/******************************************************
 *                   Enumerations
 ******************************************************/

/**
 * \addtogroup group_smartcoex_enums
 * \{
 */

/**
 * Enumeration of SmartCoex Interface Types
 */
typedef enum
{
    CY_SMARTCOEX_INTERFACE_TYPE_STA = 0,                                   /**< STA or Client interface.*/
    /* Should be the last entry. */
    CY_SMARTCOEX_INTERFACE_TYPE_DEFAULT = CY_SMARTCOEX_INTERFACE_TYPE_STA, /**< Default interface. */
} cy_smartcoex_wifi_interface_t;

/**
 * Enumeration of LE Scan Priority
 */
typedef enum
{
    CY_SMARTCOEX_LESCAN_PRIORITY_LOW = 0,        /**< Low priority.    */
    CY_SMARTCOEX_LESCAN_PRIORITY_MEDIUM,         /**< Medium priority. */
    CY_SMARTCOEX_LESCAN_PRIORITY_HIGH            /**< High priority.   */
} cy_smartcoex_lescan_priority_t;

/** \} group_smartcoex_enums */

/******************************************************
 *                   Structures
 ******************************************************/
/**
 * \addtogroup group_smartcoex_structs
 * \{
 */

/**
 * Smart Coex WLAN configuration parameters
 */
typedef struct
{
    cy_smartcoex_wifi_interface_t interface;    /**< Wi-Fi interface on which to set up the Smart Coex configuration. */
} cy_smartcoex_wifi_config_t;

/**
 * Smart Coex BT configuration parameters.
 */
typedef struct
{
    /**
     * BT scan priority.
     */
    cy_smartcoex_lescan_priority_t scan_priority;

    /**
     * BT scan interval.
     *
     * Units: slots (1 slot = 0.625 ms)
     * Range: 4-16384
     */
    uint16_t    scan_int;

    /**
     * BT scan window.
     *
     * Units: slots (1 slot = 0.625 ms)
     * Range: 4-16384
     */
    uint16_t    scan_win;

    /**
     * BT coex status callback.
     *
     * Callback registered with BT stack to inform the status of the Vendor-specific command issued for setting the coex config.
     */
    btcoex_cb_t btcoex_cb;
} cy_smartcoex_bt_config_t;

/** \} group_smartcoex_structs */

/******************************************************
 *                   Functions
 ******************************************************/

/**
 * \addtogroup group_smartcoex_functions
 * \{
 */

/**
 * Configures the Wi-Fi and BT parameters for Smart Coex.
 *
 * This function configures Wi-Fi BT Smart Coex based on the inputs provided.
 * Note: This should be called every time LE scan interval and/or scan window is updated.
 *
 * @param[in]  wifi_config  : Pointer to the Wi-Fi config structure.
 * @param[in]  bt_config    : Pointer to the BT config structure.
 *
 * @return status           : CY_RSLT_SUCCESS on success; an error code on failure.
 */
cy_rslt_t cy_smartcoex_config(cy_smartcoex_wifi_config_t *wifi_config, cy_smartcoex_bt_config_t *bt_config);

/** \} group_smart_functions */

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* ifndef INCLUDED_CY_SMARTCOEX_INTERFACE_H_ */
