/*
 * Copyright 2020, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software"), is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
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
#include "cy_wcm.h"
#include "cy_result_mw.h"
#include "cy_log.h"

#include "whd.h"
#include "whd_wifi_api.h"

cy_rslt_t cy_wcm_get_whd_interface(cy_wcm_interface_t interface_type, whd_interface_t *whd_iface);

cy_rslt_t set_wifi_coex_config(cy_smartcoex_wifi_config_t *wifi_config, whd_coex_config_t *coex_config)
{
    cy_rslt_t res;
    uint32_t result;
    cy_wcm_interface_t interface;
    whd_interface_t whd_iface;

    if(wifi_config->interface != CY_SMARTCOEX_INTERFACE_TYPE_STA)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "Interface type [0x%X] not supported\n", (unsigned int)wifi_config->interface);
        return CY_RSLT_MW_BADARG;
    }

    interface = CY_WCM_INTERFACE_TYPE_STA;

    res = cy_wcm_get_whd_interface(interface, &whd_iface);
    if(res != CY_RSLT_SUCCESS)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "cy_wcm_get_whd_interface failed with error:[0x%X]\n", (unsigned int)res);
        return res;
    }

    result = whd_wifi_set_coex_config(whd_iface, coex_config);
    if(result != WHD_SUCCESS)
    {
        cy_smartcoex_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "whd_wifi_set_coex_config failed with error:[0x%X]\n", (unsigned int)result);
        return result;
    }

    return CY_RSLT_SUCCESS;
}
