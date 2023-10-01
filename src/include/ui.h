#pragma once

#include "raylib.h"
#include "raygui.h"
#include "background.h"
#include <unordered_map>
#include <string>

class UI {
public:
private:
    struct Button { Rectangle rect; std::string text; };
    struct DummyRect { Rectangle rect; std::string text; };
    struct Drowdown { Rectangle rect; std::string text; int active; bool editMode; };

    std::unordered_map<std::string, Button> buttons;
    std::unordered_map<std::string, DummyRect> dummyRects;
    std::unordered_map<std::string, Drowdown> drowdowns;
    
    Background background;
};