#pragma once

#include "ui_controler.hpp"
#include "main_ui.hpp"

extern MainUI main_ui;
extern UIControler ui_controller;

void init_after_gui_thread_ready();