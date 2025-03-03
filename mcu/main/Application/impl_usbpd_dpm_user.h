#include "cmsis_os.h"
#include "main.h"
#include "usbpd_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_user.h"
#include "usbpd_pdo_defs.h"
#include "usbpd_pwr_if.h"
#include "usbpd_pwr_user.h"



#ifdef __cplusplus
extern "C" {
#endif


void DPM_USER_DEBUG_TRACE(int _PORT_, const char *format, ...);

void impl_USBPD_DPM_UserCableDetection(uint8_t PortNum, USBPD_CAD_EVENT State);
void impl_USBPD_DPM_Notification(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal);
void impl_USBPD_DPM_GetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size);
void impl_USBPD_DPM_SetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t Size);
void impl_USBPD_DPM_SNK_EvaluateCapabilities(uint8_t PortNum, uint32_t *PtrRequestData, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObjectType);

#ifdef __cplusplus
}
#endif