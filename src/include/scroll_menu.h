#pragma once

#include <memory>
#include <string>
#include "raylib.h"

class UI;

class ScrollMenu {
public:
    static std::shared_ptr<ScrollMenu> create(Rectangle bounds, std::string title, std::shared_ptr<UI> ui);

    void setEnabled(bool enabled);

    void update();

private:
    bool enabled = true;
    std::string title;
    Rectangle bounds;
    std::shared_ptr<UI> ui;

    friend class UI;
};