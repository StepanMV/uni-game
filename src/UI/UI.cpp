#include "ui.h"

#include "raygui.h"
#include "style_cyber.h"
#include <stdexcept>
#include <iostream>
#include "game.h"
#include "particle.h"

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

UIBuilder &UIBuilder::addGroupBox(std::string ID, GroupBoxData groupBoxData) {
    groupBoxData.rect.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    groupBoxData.rect.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    groupBoxData.rect.width *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    groupBoxData.rect.height *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    ui->groupBoxes[ID] = groupBoxData;
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

UIBuilder &UIBuilder::addSliderBar(std::string ID, SliderBarData sliderBarData) {
    sliderBarData.rect.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    sliderBarData.rect.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    sliderBarData.rect.width *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    sliderBarData.rect.height *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    ui->sliderBars[ID] = sliderBarData;
    return *this;
}

UIBuilder &UIBuilder::addObject(std::string ID, std::shared_ptr<Particle> object, bool enabled) {
    object->destroy();
    object->transform->pos.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    object->transform->pos.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    object->transform->size.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    object->transform->size.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    ui->objects[ID] = std::make_pair(enabled, object);
    return *this;
}

UIBuilder &UIBuilder::addScrollMenu(std::string ID, Rectangle rect, std::string text, std::shared_ptr<UI> ui, bool enabled) {
    rect.x *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    rect.y *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    rect.width *= Game::settings->readDouble("Runtime", "screenCoefW", 1);
    rect.height *= Game::settings->readDouble("Runtime", "screenCoefH", 1);
    this->ui->scrollMenus[ID] = ScrollMenu::create(rect, text, ui);
    this->ui->scrollMenus[ID]->setEnabled(enabled);
    return *this;
}

UIBuilder &UIBuilder::addSubUI(std::string ID, std::shared_ptr<UI> ui, bool enabled) {
    this->ui->subUIs[ID] = ui;
    this->ui->subUIs[ID]->setEnabled(enabled);
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
    if (!enabled) return;
    bool lock = false;
    for (auto &dropdown : dropdowns) {
        if (!dropdown.second.enabled) continue;
        if (dropdown.second.editMode) {
            lock = true;
            break;
        }
    }

    if (lock) GuiLock();

    for (auto &button : buttons) {
        if (!button.second.enabled) continue;
        prevButtonStates[button.first] = buttonStates[button.first];
        buttonStates[button.first] = GuiButton(button.second.rect, button.second.text.c_str());
        if (buttonStates[button.first] && !prevButtonStates[button.first]) buttonCallbacks[button.first]();
    }

    for (auto &dummyRect : dummyRects) {
        if (!dummyRect.second.enabled) continue;
        GuiDummyRec(dummyRect.second.rect, dummyRect.second.text.c_str());
    }

    for (auto &groupBox : groupBoxes) {
        if (!groupBox.second.enabled) continue;
        GuiGroupBox(groupBox.second.rect, groupBox.second.text.c_str());
    }

    for (auto &sliderBar : sliderBars) {
        if (!sliderBar.second.enabled) continue;
        GuiSliderBar(sliderBar.second.rect, sliderBar.second.textL.c_str(), sliderBar.second.textR.c_str(), sliderBar.second.value, sliderBar.second.minValue, sliderBar.second.maxValue);
    }

    for (auto &bar : bars) {
        if (!bar.second.enabled) continue;
        barPercentages[bar.first] = GuiProgressBar(bar.second.rect, bar.second.text.c_str(), nullptr, bar.second.value, bar.second.minValue, bar.second.maxValue);
    }

    for (auto &object : objects) {
        if (!object.second.first) continue;
        object.second.second->renderer->render();
    }

    for (auto &scrollMenu : scrollMenus) {
        scrollMenu.second->update();
    }

    for (auto &subUI : subUIs) {
        subUI.second->update();
    }

    for (auto &dropdown : dropdowns) {
        if (!dropdown.second.enabled) continue;
        if(GuiDropdownBox(dropdown.second.rect, dropdown.second.text.c_str(), &dropdown.second.active, dropdown.second.editMode)) {
            dropdownStates[dropdown.first] = dropdown.second.active;
            dropdown.second.editMode = !dropdown.second.editMode;
        }
    }

    GuiUnlock();
}

void UI::setEnabled(bool enabled, std::string ID) {
    if (ID.empty()) {
        this->enabled = enabled;
        if(!enabled) for (auto &button : buttons) buttonStates[button.first] = false;
    } else {
        if (buttons.find(ID) != buttons.end()) {
            buttons.at(ID).enabled = enabled;
            if(!enabled) buttonStates.at(ID) = false;
        } else if (dummyRects.find(ID) != dummyRects.end()) {
            dummyRects.at(ID).enabled = enabled;
        } else if (dropdowns.find(ID) != dropdowns.end()) {
            dropdowns.at(ID).enabled = enabled;
        } else if (bars.find(ID) != bars.end()) {
            bars.at(ID).enabled = enabled;
        } else if (objects.find(ID) != objects.end()) {
            objects.at(ID).first = enabled;
        } else {
            throw std::runtime_error("Element with ID " + ID + " does not exist");
        }
    }
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

int UI::getBarPercentage(std::string ID) const {
    if (barPercentages.find(ID) == barPercentages.end()) throw std::runtime_error("Bar with ID " + ID + " does not exist");
    return barPercentages.at(ID);
}

float UI::getSliderValue(std::string ID) const {
    if (sliderBars.find(ID) == sliderBars.end()) throw std::runtime_error("Slider bar with ID " + ID + " does not exist");
    return *sliderBars.at(ID).value;
}

std::shared_ptr<Particle> UI::getObject(std::string ID) const {
    if (objects.find(ID) != objects.end()) return objects.at(ID).second;
    return nullptr;
}

std::shared_ptr<UI> UI::getSubUI(std::string ID) const {
    if (scrollMenus.find(ID) != scrollMenus.end()) {
        return scrollMenus.at(ID)->ui;
    }
    if (subUIs.find(ID) != subUIs.end()) {
        return subUIs.at(ID);
    }
    return nullptr;
}
