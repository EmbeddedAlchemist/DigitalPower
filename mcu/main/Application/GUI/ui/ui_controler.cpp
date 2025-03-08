#include "ui_controler.hpp"

void UIControler::init(UI &initial_ui, osMessageQId qid) {
    load(initial_ui);
    queue_id = qid;
    itf.set_brightness(.0f);
    itf.first_page();
    do {
        itf.clear();
        itf.update();
    } while (itf.next_page());
    itf.set_brightness(1.f);
}

void UIControler::draw() {
    if (to_load) {
        if(current_ui)
            current_ui->on_unmount();
        current_ui = to_load;
        to_load    = nullptr;
        current_ui->on_mount(&itf);
    }

    if (current_ui == nullptr)
        return;

    UI::InputEvent event;
    while (osStatus_t::osOK == osMessageQueueGet(queue_id, &event, nullptr, 0))
        current_ui->on_input(event);

    current_ui->before_draw();
    itf.first_page();
    do{
        itf.clear();
        current_ui->on_draw();
        itf.update();
    } while (itf.next_page());
}

void UIControler::load(UI &ui) {
    to_load = &ui;
}

void UIControler::on_input(UI::InputEvent event) {
    osMessageQueuePut(queue_id, reinterpret_cast<void *>(&event) , 0, 0);
}
