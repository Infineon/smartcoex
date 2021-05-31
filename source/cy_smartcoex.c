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

#include "cy_smartcoex.h"
#include "cy_smartcoex_priv.h"
#include "cy_smartcoex_log.h"
#include "cy_result_mw.h"
#include "cy_log.h"

#include "whd_types.h"

/* BT coex parameters for Low priority */
#define SMARTCOEX_DUTY_CYCLE_LOW                25 // in percentage
#define SMARTCOEX_SMALL_INTERVAL_GRANT_LOW      4
#define SMARTCOEX_MAX_SCAN_WINDOW_LOW           48 // in slots

/* BT coex parameters for Medium priority */
#define SMARTCOEX_DUTY_CYCLE_MEDIUM             50 // in percentage
#define SMARTCOEX_SMALL_INTERVAL_GRANT_MEDIUM   2
#define SMARTCOEX_MAX_SCAN_WINDOW_MEDIUM        48 // in slots

/* BT coex parameters for High priority */
#define SMARTCOEX_DUTY_CYCLE_HIGH               90 // in percentage
#define SMARTCOEX_SMALL_INTERVAL_GRANT_HIGH     1
#define SMARTCOEX_MAX_SCAN_WINDOW_HIGH          48 // in slots

/* BT scan window parameter range */
#define SMARTCOEX_BT_SCAN_WINDOW_RANGE_LOW      4     // in slots
#define SMARTCOEX_BT_SCAN_WINDOW_RANGE_HIGH     16384 // in slots

/* BT scan interval parameter range */
#define SMARTCOEX_BT_SCAN_INTERVAL_RANGE_LOW    4     // in slots
#define SMARTCOEX_BT_SCAN_INTERVAL_RANGE_HIGH   16384 // in slots

/**
 * Scan parameters to be passed to BT stack for Coex
 */
typedef struct
{
    /*
     * Allowed maximum scan window size, above which host specified scanInterval
     * and scanWindow are re-adjusted.
     *
     * Units: slots (1 slot = 0.625 ms)
     * Range: 4-16384
     */
    uint16_t maxScanWindow;

    /**
     * The upper bound of of the percentage of the scan time that is given high priority.
     *
     * Units: percentage
     * Range: 0 - 100
     */
    uint8_t  scanDutyCycle;

    /**
     * When host specified scan interval is smaller than maxScanWindow,
     * every <smallIntervalGrant>'th scan window is given high priority.
     * For e.g., if smallIntervalGrant is 4, every 4th window is given high priority.
     *
     * Units: none
     * Range: 1 - 255
     */
    uint8_t  smallIntervalGrant;
} le_scan_param;

static le_scan_param     param;
static whd_coex_config_t whd_coex_config;

static bool is_params_valid(cy_smartcoex_wifi_config_t *wifi_config, cy_smartcoex_bt_config_t *bt_config)
{
    if(wifi_config->interface != CY_SMARTCOEX_INTERFACE_TYPE_STA)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "Invalid Wi-Fi interface type Must be CY_SMARTCOEX_INTERFACE_TYPE_STA. \n");
        return false;
    }

    if(bt_config->btcoex_cb == NULL)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "BT coex status callback cannot be NULL.\n");
        return false;
    }

    if(bt_config->scan_int < SMARTCOEX_BT_SCAN_INTERVAL_RANGE_LOW || bt_config->scan_int > SMARTCOEX_BT_SCAN_INTERVAL_RANGE_HIGH)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "Invalid Scan Interval. Must be in the range of %d to %d slots. \n",
                              SMARTCOEX_BT_SCAN_INTERVAL_RANGE_LOW, SMARTCOEX_BT_SCAN_INTERVAL_RANGE_HIGH);
        return false;
    }

    if(bt_config->scan_win < SMARTCOEX_BT_SCAN_WINDOW_RANGE_LOW || bt_config->scan_win > SMARTCOEX_BT_SCAN_WINDOW_RANGE_HIGH)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "Invalid Scan Window. Must be in the range of %d to %d slots. \n",
                              SMARTCOEX_BT_SCAN_WINDOW_RANGE_LOW, SMARTCOEX_BT_SCAN_WINDOW_RANGE_HIGH);
        return false;
    }

    if(bt_config->scan_win > bt_config->scan_int)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "Invalid Scan Window. Must be smaller than Scan Interval:%lu. \n", bt_config->scan_int);
        return false;
    }

    return true;
}

static int set_whd_coex_config(cy_smartcoex_bt_config_t *bt_config)
{
    whd_coex_config.le_scan_params.priority = (uint16_t)bt_config->scan_priority;

    whd_coex_config.le_scan_params.scan_int = bt_config->scan_int;
    whd_coex_config.le_scan_params.scan_win = bt_config->scan_win;

    switch(bt_config->scan_priority)
    {
        case CY_SMARTCOEX_LESCAN_PRIORITY_LOW:
            whd_coex_config.le_scan_params.duty_cycle = SMARTCOEX_DUTY_CYCLE_LOW;
            whd_coex_config.le_scan_params.max_win    = SMARTCOEX_MAX_SCAN_WINDOW_LOW;
            whd_coex_config.le_scan_params.int_grant  = SMARTCOEX_SMALL_INTERVAL_GRANT_LOW;
            break;

        case CY_SMARTCOEX_LESCAN_PRIORITY_MEDIUM:
            whd_coex_config.le_scan_params.duty_cycle = SMARTCOEX_DUTY_CYCLE_MEDIUM;
            whd_coex_config.le_scan_params.max_win    = SMARTCOEX_MAX_SCAN_WINDOW_MEDIUM;
            whd_coex_config.le_scan_params.int_grant  = SMARTCOEX_SMALL_INTERVAL_GRANT_MEDIUM;
            break;

        case CY_SMARTCOEX_LESCAN_PRIORITY_HIGH:
            whd_coex_config.le_scan_params.duty_cycle = SMARTCOEX_DUTY_CYCLE_HIGH;
            whd_coex_config.le_scan_params.max_win    = SMARTCOEX_MAX_SCAN_WINDOW_HIGH;
            whd_coex_config.le_scan_params.int_grant  = SMARTCOEX_SMALL_INTERVAL_GRANT_HIGH;
            break;

        default:
            cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "Invalid LE scan priority. Must be 0, 1 or 2. \n");
            return -1;
    }

    return 0;
}

cy_rslt_t cy_smartcoex_config(cy_smartcoex_wifi_config_t *wifi_config, cy_smartcoex_bt_config_t *bt_config)
{
    wiced_result_t res;

    if(!is_params_valid(wifi_config, bt_config))
    {
        return CY_RSLT_MW_BADARG;
    }

    if(set_whd_coex_config(bt_config) == -1)
    {
        return CY_RSLT_MW_BADARG;
    }

    param.maxScanWindow = whd_coex_config.le_scan_params.max_win;
    param.scanDutyCycle = whd_coex_config.le_scan_params.duty_cycle;
    param.smallIntervalGrant = whd_coex_config.le_scan_params.int_grant;

    res = wiced_bt_dev_vendor_specific_command(BTHCI_CMD_VS_OCF_BTCX_LESCAN,
            sizeof(le_scan_param), (uint8_t*)&param, bt_config->btcoex_cb);
    if(res == WICED_BT_BUSY)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "BT stack is busy. Command not sent.\n");
        return CY_RSLT_MW_ERROR;
    }
    else if(res != WICED_BT_SUCCESS && res != WICED_BT_PENDING)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "wiced_bt_dev_vendor_specific_command failed with error:[0x%X]\n", (unsigned int)res);
        return CY_RSLT_MW_ERROR;
    }

    return set_wifi_coex_config(wifi_config, &whd_coex_config);
}
