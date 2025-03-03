#include "impl_usbpd_dpm_user.h"

#include "onboard_resource.hpp"
#include "stm32g4xx_ll_ucpd.h"
#include "ucpd.h"

#include <cstdlib>
#include <cstring>

extern "C" void DPM_USER_DEBUG_TRACE(int _PORT_, const char *format, ...) {
    va_list args;
    va_start(args, format);
    console.arg_log('D', format, args);
    va_end(args);
}

/**
 * @brief  UserCableDetection reporting events on a specified port from CAD layer.
 * @param  PortNum The handle of the port
 * @param  State CAD state
 * @retval None
 */
extern "C" void impl_USBPD_DPM_UserCableDetection(uint8_t PortNum, USBPD_CAD_EVENT State) {
    switch (State) {
    case USBPD_CAD_EVENT_DETACHED:
        pd.impl_cable_detached();
        break;
    case USBPD_CAD_EVENT_ATTACHED:
        pd.impl_cable_attached();
        break;
    default:
        console.debug("Unhandled Cable Detection: %d.", State);
    }
}

/**
 * @brief  Callback function called by PE to inform DPM about PE event.
 * @param  PortNum The current port number
 * @param  EventVal @ref USBPD_NotifyEventValue_TypeDef
 * @retval None
 */
extern "C" void impl_USBPD_DPM_Notification(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal) {
    /* USER CODE BEGIN USBPD_DPM_Notification */
    /* Manage event notified by the stack? */
    switch (EventVal) {
    case USBPD_NOTIFY_STATE_SNK_READY:
        pd.impl_snk_ready();
        break;
    case USBPD_NOTIFY_USBSTACK_START:
        pd.impl_stack_start();
        break;
    case USBPD_NOTIFY_USBSTACK_STOP:
        pd.impl_stack_stop();
        break;
    default:
        console.debug("Unhandled Notification: %d", EventVal);
        break;
    }


    /* USER CODE END USBPD_DPM_Notification */
}

/**
 * @brief  DPM callback to allow PE to retrieve information from DPM/PWR_IF.
 * @param  PortNum Port number
 * @param  DataId  Type of data to be updated in DPM based on @ref USBPD_CORE_DataInfoType_TypeDef
 * @param  Ptr     Pointer on address where DPM data should be written (u8 pointer)
 * @param  Size    Pointer on nb of u8 written by DPM
 * @retval None
 */
extern "C" void impl_USBPD_DPM_GetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size) {
    switch (DataId) {
    default:
        console.debug("Unhandled get data info: %d", DataId);
        break;
    }
}

/**
 * @brief  DPM callback to allow PE to update information in DPM/PWR_IF.
 * @param  PortNum Port number
 * @param  DataId  Type of data to be updated in DPM based on @ref USBPD_CORE_DataInfoType_TypeDef
 * @param  Ptr     Pointer on the data
 * @param  Size    Nb of bytes to be updated in DPM
 * @retval None
 */
void impl_USBPD_DPM_SetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t Size) {
    /* USER CODE BEGIN USBPD_DPM_SetDataInfo */
    /* Check type of information targeted by request */
    switch (DataId) {

    case USBPD_CORE_DATATYPE_RCV_SRC_PDO:
        pd.impl_rcv_src_pdo(Ptr, Size);
        break;
    default:
        DPM_USER_DEBUG_TRACE(PortNum, "Unhandled set data info: %d", DataId);
        break;
    }
    /* USER CODE END USBPD_DPM_SetDataInfo */
}

/**
 * @brief  Evaluate received Capabilities Message from Source port and prepare the request message
 * @param  PortNum         Port number
 * @param  PtrRequestData  Pointer on selected request data object
 * @param  PtrPowerObjectType  Pointer on the power data object
 * @retval None
 */
extern "C" void impl_USBPD_DPM_SNK_EvaluateCapabilities(uint8_t PortNum, uint32_t *PtrRequestData, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObjectType) {
    pd.impl_snk_eval_capabilities(PtrRequestData, PtrPowerObjectType);
}
