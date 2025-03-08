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

extern "C" void impl_USBPD_DPM_Notification(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal) {
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
}


extern "C" void impl_USBPD_DPM_GetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size) {
    switch (DataId) {
    default:
        console.debug("Unhandled get data info: %d", DataId);
        break;
    }
}

void impl_USBPD_DPM_SetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t Size) {
    switch (DataId) {
    case USBPD_CORE_DATATYPE_RCV_SRC_PDO:
        pd.impl_rcv_src_pdo(Ptr, Size);
        break;
    default:
        console.debug("Unhandled set data info: %d", DataId);
        break;
    }
}

extern "C" void impl_USBPD_DPM_SNK_EvaluateCapabilities(uint8_t PortNum, uint32_t *PtrRequestData, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObjectType) {
    pd.impl_snk_eval_capabilities(PtrRequestData, PtrPowerObjectType);
}
