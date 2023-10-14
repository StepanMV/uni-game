#include "ui.h"

#include "raygui.h"
#include <stdexcept>
#include <iostream>
#include "game.h"

UIBuilder &UIBuilder::addButton(std::string ID, ButtonData buttonData, std::function<void()> callback) {
    buttonData.rect.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    buttonData.rect.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    buttonData.rect.width *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    buttonData.rect.height *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    ui->buttons[ID] = buttonData;
    ui->buttonCallbacks[ID] = callback;
    return *this;
}

UIBuilder::UIBuilder() : ui(std::make_shared<UI>()) {}

UIBuilder &UIBuilder::addDummyRect(std::string ID, DummyRectData dummyRectData) {
    dummyRectData.rect.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    dummyRectData.rect.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    dummyRectData.rect.width *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    dummyRectData.rect.height *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    ui->dummyRects[ID] = dummyRectData;
    return *this;
}

UIBuilder &UIBuilder::addDropdown(std::string ID, DropdownData dropdownData) {
    dropdownData.rect.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    dropdownData.rect.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    dropdownData.rect.width *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    dropdownData.rect.height *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    ui->dropdowns[ID] = dropdownData;
    return *this;
}

UIBuilder &UIBuilder::addBar(std::string ID, BarData barData) {
    barData.rect.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    barData.rect.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    barData.rect.width *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    barData.rect.height *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    barData.text += " ";
    ui->bars[ID] = barData;
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

void UI::update() {
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

    for (auto &bar : bars) {
        barPercentages[bar.first] = GuiProgressBar(bar.second.rect, bar.second.text.c_str(), nullptr, bar.second.value, bar.second.minValue, bar.second.maxValue);
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

void UI::setBarValue(std::string ID, float *value) {
    if (bars.find(ID) == bars.end()) throw std::runtime_error("Bar with ID " + ID + " does not exist");
    bars.at(ID).value = value;
}

int UI::getDropdownValue(std::string ID) const {
    if (dropdownStates.find(ID) == dropdownStates.end()) throw std::runtime_error("Dropdown with ID " + ID + " does not exist");
    return dropdownStates.at(ID);
}