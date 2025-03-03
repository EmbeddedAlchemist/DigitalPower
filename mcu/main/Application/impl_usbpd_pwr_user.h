#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void PWR_DEBUG_TRACE(int _PORT_, const char * format, ...);
int32_t impl_BSP_USBPD_PWR_VBUSGetVoltage(uint32_t Instance, uint32_t *pVoltage);
int32_t impl_BSP_USBPD_PWR_VBUSGetCurrent(uint32_t Instance, int32_t *pCurrent);

#ifdef __cplusplus
}
#endif
