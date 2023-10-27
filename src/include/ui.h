#pragma once

#include "raylib.h"
#include "background.h"
#include "scroll_menu.h"
#include <unordered_map>
#include <string>
#include <functional>

struct ButtonData { Rectangle rect; std::string text; bool enabled = true; };
struct DummyRectData { Rectangle rect; std::string text; bool enabled = true; };
struct DropdownData { Rectangle rect; std::string text; int active; bool editMode; bool enabled = true; };
struct BarData { Rectangle rect; std::string text; float* value; float minValue; float maxValue; bool enabled = true; };
struct SliderBarData { Rectangle rect; std::string textL; std::string textR; float minValue; float maxValue; bool enabled = true; };
struct GroupBoxData { Rectangle rect; std::string text; bool enabled = true; };
class Particle;

class UI {
public:
    UI();
    void update();

    void setEnabled(bool enabled, std::string ID = "");

    Color getBackgroundColor() const;
    bool isInsideUI(Vec2 point) const;

    bool isButtonPressed(std::string ID) const;
    bool isButtonReleased(std::string ID) const;
    bool isButtonHeld(std::string ID) const;

    void setDropdownValue(std::string ID, int value);
    int getDropdownValue(std::string ID) const;
    void setBarPointer(std::string ID, float* value);
    int getBarPercentage(std::string ID) const;
    void setSliderValue(std::string ID, float value);
    float getSliderValue(std::string ID) const;
    std::shared_ptr<Particle> getObject(std::string ID) const;
    std::shared_ptr<UI> getSubUI(std::string ID) const;

private:
    bool enabled = true;

    std::unordered_map<std::string, std::function<void()>> buttonCallbacks;

    std::unordered_map<std::string, bool> prevButtonStates;
    std::unordered_map<std::string, bool> buttonStates;
    std::unordered_map<std::string, int> dropdownStates;
    std::unordered_map<std::string, int> barPercentages;
    std::unordered_map<std::string, float> sliderValues;


    std::unordered_map<std::string, ButtonData> buttons;
    std::unordered_map<std::string, DummyRectData> dummyRects;
    std::unordered_map<std::string, GroupBoxData> groupBoxes;
    std::unordered_map<std::string, DropdownData> dropdowns;
    std::unordered_map<std::string, BarData> bars;
    std::unordered_map<std::string, SliderBarData> sliderBars;
    std::unordered_map<std::string, std::pair<bool, std::shared_ptr<Particle>>> objects;
    std::unordered_map<std::string, std::shared_ptr<ScrollMenu>> scrollMenus;
    std::unordered_map<std::string, std::shared_ptr<UI>> subUIs;

    friend class UIBuilder;
    friend class ScrollMenu;
};

class UIBuilder {
public:
    UIBuilder();
    UIBuilder& addButton(std::string ID, ButtonData buttonData, std::function<void()> callback = []() {});
    UIBuilder& addDummyRect(std::string ID, DummyRectData dummyRectData);
    UIBuilder& addGroupBox(std::string ID, GroupBoxData groupBoxData);
    UIBuilder& addDropdown(std::string ID, DropdownData dropdownData);
    UIBuilder& addBar(std::string ID, BarData barData);
    UIBuilder& addSliderBar(std::string ID, SliderBarData sliderBarData);
    UIBuilder& addObject(std::string ID, std::shared_ptr<Particle> object, bool enabled = true);
    UIBuilder& addScrollMenu(std::string ID, Rectangle rect, std::string text, std::shared_ptr<UI> ui, bool enabled = true);
    UIBuilder& addSubUI(std::string ID, std::shared_ptr<UI> ui, bool enabled = true);
    std::shared_ptr<UI> build();

private:
    std::shared_ptr<UI> ui;
};