#pragma once

#include "cmsis_os.h"
#include "drawing_interface.hpp"
#include "ui.hpp"

using namespace exgraphics;

class UIControler {
  private:
    DrawingInterface &itf;
    UI *current_ui;
    UI *to_load = nullptr;
    osMessageQId queue_id;

  public:
    inline UIControler(DrawingInterface &itf)
        : itf(itf) {}

    void init(UI &initial_ui, osMessageQId queue_id);
    void draw();
    void load(UI &ui);
    void on_input(UI::InputEvent event);
};