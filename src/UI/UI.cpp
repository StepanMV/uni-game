#include "ui.h"

#include "raygui.h"
#include "style_cyber.h"
#include <stdexcept>
#include <iostream>

UIBuilder &UIBuilder::addButton(std::string ID, ButtonData buttonData, std::function<void()> callback) {
    ui->buttons[ID] = buttonData;
    ui->buttonCallbacks[ID] = callback;
    return *this;
}

UIBuilder::UIBuilder() : ui(std::make_shared<UI>()) {}

UIBuilder &UIBuilder::addDummyRect(std::string ID, DummyRectData dummyRectData) {
    ui->dummyRects[ID] = dummyRectData;
    return *this;
}

UIBuilder &UIBuilder::addDropdown(std::string ID, DropdownData dropdownData) {
    ui->dropdowns[ID] = dropdownData;
    return *this;
}

std::shared_ptr<UI> UIBuilder::build()  {
    for (auto &dropdown : ui->dropdowns) {
        ui->dropdownStates[dropdown.first] = dropdown.second.active;
    }

    for (auto &button : ui->buttons) {
        ui->prevButtonStates[button.first] = false;
        ui->buttonStates[button.first] = false;
    }

    return ui;
}

UI::UI() {
    GuiLoadStyleCyber();
}

void UI::update()
{
    bool lock = false;
    for (auto &dropdown : dropdowns) {
        if (dropdown.second.editMode) {
            lock = true;
            break;
        }
    }

    if (lock) GuiLock();

    for (auto &button : buttons) {
        prevButtonStates[button.first] = buttonStates[button.first];
        buttonStates[button.first] = GuiButton(button.second.rect, button.second.text.c_str());
        if (buttonStates[button.first] && !prevButtonStates[button.first]) buttonCallbacks[button.first]();
    }

    for (auto &dummyRect : dummyRects) {
        GuiDummyRec(dummyRect.second.rect, dummyRect.second.text.c_str());
    }

    for (auto &dropdown : dropdowns) {
        if(GuiDropdownBox(dropdown.second.rect, dropdown.second.text.c_str(), &dropdown.second.active, dropdown.second.editMode)) {
            dropdown.second.editMode = !dropdown.second.editMode;
        }
    }

    GuiUnlock();
}

Color UI::getBackgroundColor() const {
    return GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

bool UI::isButtonPressed(std::string ID) const
{
    if (buttonStates.find(ID) == buttonStates.end()) return false;
    return buttonStates.at(ID) && !prevButtonStates.at(ID);
}

bool UI::isButtonReleased(std::string ID) const {
    if (buttonStates.find(ID) == buttonStates.end()) return false;
    return !buttonStates.at(ID) && prevButtonStates.at(ID);
}

bool UI::isButtonHeld(std::string ID) const {
    if (buttonStates.find(ID) == buttonStates.end()) return false;
    return buttonStates.at(ID);
}

int UI::getDropdownValue(std::string ID) const {
    if (dropdownStates.find(ID) == dropdownStates.end()) throw std::runtime_error("Dropdown with ID " + ID + " does not exist");
    return dropdownStates.at(ID);
}
