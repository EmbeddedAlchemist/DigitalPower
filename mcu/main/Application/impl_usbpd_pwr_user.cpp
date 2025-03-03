#include "impl_usbpd_pwr_user.h"
#include "onboard_resource.hpp"
#include "usbpd_pwr_user.h"



extern "C" {

void PWR_DEBUG_TRACE(int _PORT_, const char *format, ...) {
    va_list args;
    va_start(args, format);
    console.arg_log('D', format, args);
    va_end(args);
}

/**
 * @brief  Get actual voltage level measured on the VBUS line.
 * @param  Instance Type-C port identifier
 *         This parameter can be take one of the following values:
 *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
 * @param  pVoltage Pointer on measured voltage level (in mV)
 * @retval BSP status
 */
int32_t impl_BSP_USBPD_PWR_VBUSGetVoltage(uint32_t Instance, uint32_t *pVoltage) {
    int32_t ret  = BSP_ERROR_NONE;
    uint32_t val = power_sample.get_pd_voltage() * 1000;

    if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pVoltage)) {
        ret = BSP_ERROR_WRONG_PARAM;
    }
    *pVoltage = val;
    return ret;
}

int32_t impl_BSP_USBPD_PWR_VBUSGetCurrent(uint32_t Instance, int32_t *pCurrent) {
    int32_t ret = BSP_ERROR_NONE;

    if ((Instance >= USBPD_PWR_INSTANCES_NBR) || (NULL == pCurrent)) {
        ret = BSP_ERROR_WRONG_PARAM;
    } else {
        *pCurrent = power_sample.get_in_current() * 1000;
    }
    return ret;
}
}
