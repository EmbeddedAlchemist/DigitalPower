#include "gui_thread.hpp"
#include "onboard_resource.hpp"
#include "ui_resource.hpp"

extern "C" void gui_thread(void *unused) {

    osThreadFlagsWait(gui_thread_signal_display_device_ready, osFlagsWaitAll, osWaitForever);
    init_after_gui_thread_ready();

    while (true) {
        ui_controller.draw();
    }
}
