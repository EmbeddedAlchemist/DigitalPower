#include <STC8H.H>

void isp_btn_init() {
    P3M0 &= ~0x04;
    P3M1 &= ~0x04;
    EX0 = 1;
    IT0 = 1;
}

void isp_btn_on_pressed() {
    IAP_CONTR = 0x60;
}

void int0_handler() interrupt INT0_VECTOR {
    if (IE == 0)
        return;
    isp_btn_on_pressed();
}