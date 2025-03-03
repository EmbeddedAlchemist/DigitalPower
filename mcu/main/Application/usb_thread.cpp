#include "usb_thread.hpp"
#include "cmsis_os.h"
#include "onboard_resource.hpp"

extern "C" void usb_thread(void *unused) {
    osThreadFlagsWait(usb_thread_signal_ready, osFlagsWaitAll, osWaitForever);
    
    osThreadTerminate(osThreadGetId());
}