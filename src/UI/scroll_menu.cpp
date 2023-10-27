#include "scroll_menu.h"
#include "ui.h"
#include "controls.h"
#include "particle.h"
#include "raygui.h"
#include <iostream>

std::shared_ptr<ScrollMenu> ScrollMenu::create(Rectangle bounds, std::string title, std::shared_ptr<UI> ui){
    std::shared_ptr<ScrollMenu> scrollMenu = std::make_shared<ScrollMenu>();
    scrollMenu->bounds = bounds;
    scrollMenu->title = title;
    scrollMenu->ui = ui;
    scrollMenu->scroll(0);
    return scrollMenu;
}

void ScrollMenu::setEnabled(bool enabled) {
    this->enabled = enabled;
}

bool ScrollMenu::isInside(Vec2 point) const {
    return CheckCollisionPointRec(point.toRaylib(), bounds);
}

void ScrollMenu::update() {
    if (!this->enabled) return;
    ui->update();
    GuiGroupBox(bounds, title.c_str());
    float scrollValue = Controls::getMouseScroll();
    Vec2 mousePos = Controls::getMousePos();
    if(scrollValue != 0 && CheckCollisionPointRec(mousePos.toRaylib(), bounds)) scroll(scrollValue);

}

void ScrollMenu::scroll(float scrollValue) {
    for (auto& [ID, button] : ui->buttons) {
        button.rect.y += scrollValue * 10;
        button.enabled = button.rect.y > bounds.y && button.rect.y + button.rect.height < bounds.y + bounds.height;
    }
    for (auto& [ID, dummyRect] : ui->dummyRects) {
        dummyRect.rect.y += scrollValue * 10;
        dummyRect.enabled = dummyRect.rect.y > bounds.y && dummyRect.rect.y + dummyRect.rect.height < bounds.y + bounds.height;
    }
    for (auto& [ID, dropdown] : ui->dropdowns) {
        dropdown.rect.y += scrollValue * 10;
        dropdown.enabled = dropdown.rect.y > bounds.y && dropdown.rect.y + dropdown.rect.height < bounds.y + bounds.height;
    }
    for (auto& [ID, bar] : ui->bars) {
        bar.rect.y += scrollValue * 10;
        bar.enabled = bar.rect.y > bounds.y && bar.rect.y + bar.rect.height < bounds.y + bounds.height;
    }
    for (auto& [ID, object] : ui->objects) {
        object.second->transform->pos.y += scrollValue * 10;
        Vec2 pos = object.second->transform->pos - object.second->transform->size * 0.5;
        object.first = pos.y > bounds.y && pos.y + object.second->transform->size.y < bounds.y + bounds.height;
    }
}
