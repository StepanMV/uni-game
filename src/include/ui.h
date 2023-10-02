#pragma once

#include "raylib.h"
#include "raygui.h"
#include "background.h"
#include <unordered_map>
#include <string>

struct ButtonData { Rectangle rect; std::string text; };
struct DummyRectData { Rectangle rect; std::string text; };
struct DropdownData { Rectangle rect; std::string text; int active; bool editMode; };

class UI {
public:
    void update();
    void render();



private:
    std::unordered_map<std::string, ButtonData> buttons;
    std::unordered_map<std::string, DummyRectData> dummyRects;
    std::unordered_map<std::string, DropdownData> dropdowns;
    
    Background background;
};

class UIBuilder {

    UIBuilder& addButton(std::string ID, ButtonData buttonData);
    UIBuilder& addDummyRect(std::string ID, DummyRectData dummyRectData);
    UIBuilder& addDummyRect(std::string ID, DropdownData dropdownData);
    UI build() const;
    
    std::unordered_map<std::string, ButtonData> buttons;
    std::unordered_map<std::string, DummyRectData> dummyRects;
    std::unordered_map<std::string, DropdownData> dropdowns;
};