#pragma once

#include "raylib.h"
#include "background.h"
#include <unordered_map>
#include <string>
#include <functional>

struct ButtonData { Rectangle rect; std::string text; };
struct DummyRectData { Rectangle rect; std::string text; };
struct DropdownData { Rectangle rect; std::string text; int active; bool editMode; };
struct BarData { Rectangle rect; std::string text; float* value; float minValue; float maxValue; };
class Object;

class UI {
public:
    UI();
    void update();

    Color getBackgroundColor() const;

    bool isButtonPressed(std::string ID) const;
    bool isButtonReleased(std::string ID) const;
    bool isButtonHeld(std::string ID) const;

    void setBarValue(std::string ID, float* value);

    int getDropdownValue(std::string ID) const;
    int getBarPercentage(std::string ID) const;
    std::shared_ptr<Renderer> getObject(std::string ID) const;

private:
    std::unordered_map<std::string, std::function<void()>> buttonCallbacks;

    std::unordered_map<std::string, bool> prevButtonStates;
    std::unordered_map<std::string, bool> buttonStates;
    std::unordered_map<std::string, int> dropdownStates;
    std::unordered_map<std::string, int> barPercentages;

    std::unordered_map<std::string, ButtonData> buttons;
    std::unordered_map<std::string, DummyRectData> dummyRects;
    std::unordered_map<std::string, DropdownData> dropdowns;
    std::unordered_map<std::string, BarData> bars;
    std::unordered_map<std::string, std::pair<Vec2, std::shared_ptr<Renderer>>> objects;

    friend class UIBuilder;
};

class UIBuilder {
public:
    UIBuilder();
    UIBuilder& addButton(std::string ID, ButtonData buttonData, std::function<void()> callback = []() {});
    UIBuilder& addDummyRect(std::string ID, DummyRectData dummyRectData);
    UIBuilder& addDropdown(std::string ID, DropdownData dropdownData);
    UIBuilder& addBar(std::string ID, BarData barData);
    UIBuilder& addObject(std::string ID, Vec2 pos, std::shared_ptr<Renderer> object);
    std::shared_ptr<UI> build();

private:
    std::shared_ptr<UI> ui;
};