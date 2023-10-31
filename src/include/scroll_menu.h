#pragma once

#include <memory>
#include <string>
#include "raylib.h"
#include "vec2.h"

class UI;

class ScrollMenu {
public:
    static std::shared_ptr<ScrollMenu> create(Rectangle bounds, std::string title, std::shared_ptr<UI> ui);

    void setEnabled(bool enabled);
    bool isInside(Vec2 point) const;

    void update();

private:
    void scroll(float scrollValue);

    bool enabled = true;
    std::string title;
    Rectangle bounds;
    std::shared_ptr<UI> ui;

    friend class UI;
};