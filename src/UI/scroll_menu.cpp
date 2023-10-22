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
    return scrollMenu;
}

void ScrollMenu::setEnabled(bool enabled) {
    this->enabled = enabled;
}

void ScrollMenu::update() {
    if (!this->enabled) return;
    ui->update();
    GuiGroupBox(bounds, title.c_str());
    float scroll = Controls::getMouseScroll();
    Vec2 mousePos = Controls::getMousePos();
    if(scroll == 0 || !CheckCollisionPointRec(mousePos.toRaylib(), bounds)) return;
    for (auto& [ID, button] : ui->buttons) {
        button.rect.y += scroll * 10;
        button.enabled = button.rect.y > bounds.y && button.rect.y + button.rect.height < bounds.y + bounds.height;
    }
    for (auto& [ID, dummyRect] : ui->dummyRects) {
        dummyRect.rect.y += scroll * 10;
        dummyRect.enabled = dummyRect.rect.y > bounds.y && dummyRect.rect.y + dummyRect.rect.height < bounds.y + bounds.height;
    }
    for (auto& [ID, dropdown] : ui->dropdowns) {
        dropdown.rect.y += scroll * 10;
        dropdown.enabled = dropdown.rect.y > bounds.y && dropdown.rect.y + dropdown.rect.height < bounds.y + bounds.height;
    }
    for (auto& [ID, bar] : ui->bars) {
        bar.rect.y += scroll * 10;
        bar.enabled = bar.rect.y > bounds.y && bar.rect.y + bar.rect.height < bounds.y + bounds.height;
    }
    for (auto& [ID, object] : ui->objects) {
        object.second->transform->pos.y += scroll * 10;
        Vec2 pos = object.second->transform->pos - object.second->transform->size * 0.5;
        object.first = pos.y > bounds.y && pos.y + object.second->transform->size.y < bounds.y + bounds.height;
    }

}
