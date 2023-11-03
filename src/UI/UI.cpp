#include "ui.h"

#include "raygui.h"
#include "style_cyber.h"
#include <stdexcept>
#include <iostream>
#include "game.h"
#include "particle.h"
#include "audio.h"

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
    ui->sliderValues[ID] = 0;
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

    for (auto &bar : ui->bars) {
        ui->barValues[bar.first] = 0;
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
        if (button.second.visible) buttonStates[button.first] = GuiButton(button.second.rect, button.second.text.c_str());
        else buttonStates[button.first] = GuiLabelButton(button.second.rect, button.second.text.c_str());
        if (buttonStates[button.first] && !prevButtonStates[button.first]) {
            buttonCallbacks[button.first]();
            Audio::playSound("Menu_Tick");
        }
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
        GuiSliderBar(sliderBar.second.rect, sliderBar.second.textL.c_str(), sliderBar.second.textR.c_str(), &sliderValues.at(sliderBar.first), sliderBar.second.minValue, sliderBar.second.maxValue);
    }

    for (auto &bar : bars) {
        if (!bar.second.enabled) continue;
        GuiProgressBar(bar.second.rect, bar.second.text.c_str(), nullptr, &barValues.at(bar.first), 0, 1);
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
        if(GuiDropdownBox(dropdown.second.rect, dropdown.second.text.c_str(), &dropdownStates[dropdown.first], dropdown.second.editMode)) {
            dropdown.second.editMode = !dropdown.second.editMode;
            Audio::playSound("Menu_Tick");
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

bool UI::isInsideUI(Vec2 point) const {
    for (auto &button : buttons) {
        if (!button.second.enabled) continue;
        if (CheckCollisionPointRec(point.toRaylib(), button.second.rect)) return true;
    }

    for (auto &dummyRect : dummyRects) {
        if (!dummyRect.second.enabled) continue;
        if (CheckCollisionPointRec(point.toRaylib(), dummyRect.second.rect)) return true;
    }

    for (auto &groupBox : groupBoxes) {
        if (!groupBox.second.enabled) continue;
        if (CheckCollisionPointRec(point.toRaylib(), groupBox.second.rect)) return true;
    }

    for (auto &sliderBar : sliderBars) {
        if (!sliderBar.second.enabled) continue;
        if (CheckCollisionPointRec(point.toRaylib(), sliderBar.second.rect)) return true;
    }

    for (auto &bar : bars) {
        if (!bar.second.enabled) continue;
        if (CheckCollisionPointRec(point.toRaylib(), bar.second.rect)) return true;
    }

    for (auto &scrollMenu : scrollMenus) {
        if (!scrollMenu.second->enabled) continue;
        if (scrollMenu.second->isInside(point)) return true;
    }

    for (auto &subUI : subUIs) {
        if (!subUI.second->enabled) continue;
        if (subUI.second->isInsideUI(point)) return true;
    }

    for (auto &dropdown : dropdowns) {
        if (!dropdown.second.enabled) continue;
        if (CheckCollisionPointRec(point.toRaylib(), dropdown.second.rect)) return true;
    }

    return false;
}

bool UI::isInsideElement(Vec2 point, std::string ID) const {
    if (buttons.find(ID) != buttons.end()) {
        if (!buttons.at(ID).enabled) return false;
        return CheckCollisionPointRec(point.toRaylib(), buttons.at(ID).rect);
    } else if (dummyRects.find(ID) != dummyRects.end()) {
        if (!dummyRects.at(ID).enabled) return false;
        return CheckCollisionPointRec(point.toRaylib(), dummyRects.at(ID).rect);
    } else if (groupBoxes.find(ID) != groupBoxes.end()) {
        if (!groupBoxes.at(ID).enabled) return false;
        return CheckCollisionPointRec(point.toRaylib(), groupBoxes.at(ID).rect);
    } else if (sliderBars.find(ID) != sliderBars.end()) {
        if (!sliderBars.at(ID).enabled) return false;
        return CheckCollisionPointRec(point.toRaylib(), sliderBars.at(ID).rect);
    } else if (bars.find(ID) != bars.end()) {
        if (!bars.at(ID).enabled) return false;
        return CheckCollisionPointRec(point.toRaylib(), bars.at(ID).rect);
    } else if (scrollMenus.find(ID) != scrollMenus.end()) {
        if (!scrollMenus.at(ID)->enabled) return false;
        return scrollMenus.at(ID)->isInside(point);
    } else if (subUIs.find(ID) != subUIs.end()) {
        if (!subUIs.at(ID)->enabled) return false;
        return subUIs.at(ID)->isInsideElement(point, ID);
    } else if (dropdowns.find(ID) != dropdowns.end()) {
        if (!dropdowns.at(ID).enabled) return false;
        return CheckCollisionPointRec(point.toRaylib(), dropdowns.at(ID).rect);
    } else if (objects.find(ID) != objects.end()) {
        if (!objects.at(ID).first) return false;
        Vec2 size = objects.at(ID).second->transform->size;
        Vec2 pos = objects.at(ID).second->transform->pos;
        Rectangle hitbox = {pos.x - size.x / 2, pos.y - size.y / 2, size.x, size.y};
        return CheckCollisionPointRec(point.toRaylib(), hitbox);
    } else {
        throw std::runtime_error("Element with ID " + ID + " does not exist");
    }
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

void UI::setDropdownValue(std::string ID, int value) {
    if (dropdownStates.find(ID) == dropdownStates.end()) throw std::runtime_error("Dropdown with ID " + ID + " does not exist");
    dropdownStates.at(ID) = value;
}

int UI::getDropdownValue(std::string ID) const
{
    if (dropdownStates.find(ID) == dropdownStates.end()) throw std::runtime_error("Dropdown with ID " + ID + " does not exist");
    return dropdownStates.at(ID);
}

void UI::setBarValue(std::string ID, float value) {
    if (bars.find(ID) == bars.end()) throw std::runtime_error("Bar with ID " + ID + " does not exist");
    barValues.at(ID) = value;
}

int UI::getBarValue(std::string ID) const {
    if (bars.find(ID) == bars.end()) throw std::runtime_error("Bar with ID " + ID + " does not exist");
    return barValues.at(ID);
}

void UI::setSliderValue(std::string ID, float value) {
    if (sliderBars.find(ID) == sliderBars.end()) throw std::runtime_error("Slider bar with ID " + ID + " does not exist");
    sliderValues.at(ID) = value;
}

float UI::getSliderValue(std::string ID) const {
    if (sliderBars.find(ID) == sliderBars.end()) throw std::runtime_error("Slider bar with ID " + ID + " does not exist");
    return sliderValues.at(ID);
}

std::shared_ptr<CoolRenderer> UI::getObjectRenderer(std::string ID) const {
    if (objects.find(ID) != objects.end()) return std::dynamic_pointer_cast<CoolRenderer>(objects.at(ID).second->renderer);
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
