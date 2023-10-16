#pragma once

#include "raylib.h"
#include "background.h"
#include <unordered_map>
#include <string>
#include <functional>

struct ButtonData { Rectangle rect; std::string text; };
struct DummyRectData { Rectangle rect; std::string text; };
struct DropdownData { Rectangle rect; std::string text; int active; bool editMode; };

class UI {
public:
    UI();
    void update();
    //void render();

    Color getBackgroundColor() const;
    std::shared_ptr<Background> getBackground() const;

    bool isButtonPressed(std::string ID) const;
    bool isButtonReleased(std::string ID) const;
    bool isButtonHeld(std::string ID) const;

    int getDropdownValue(std::string ID) const;

private:
    std::unordered_map<std::string, std::function<void()>> buttonCallbacks;

    std::unordered_map<std::string, bool> prevButtonStates;
    std::unordered_map<std::string, bool> buttonStates;
    std::unordered_map<std::string, int> dropdownStates;

    std::unordered_map<std::string, ButtonData> buttons;
    std::unordered_map<std::string, DummyRectData> dummyRects;
    std::unordered_map<std::string, DropdownData> dropdowns;

    friend class UIBuilder;
};

class UIBuilder {
public:
    UIBuilder();
    UIBuilder& addButton(std::string ID, ButtonData buttonData, std::function<void()> callback = []() {});
    UIBuilder& addDummyRect(std::string ID, DummyRectData dummyRectData);
    UIBuilder& addDropdown(std::string ID, DropdownData dropdownData);
    std::shared_ptr<UI> build();

private:
    std::shared_ptr<UI> ui;
};