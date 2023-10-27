#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <memory>

class CppMusic {
public:
    CppMusic(std::string filepath);

    void play();
    void stop();
    void pause();
    void seek(float seconds);
    void resume();
private:
    Music data;
    std::string filepath;

    friend class Audio;
};

class Audio {
public:
    static void init(std::string folder);
    static void update();
    static void unload();
    static void setMusic(std::string ID);
    static void playSound(std::string ID);
    static void setMasterVolume(float volume);
    static void setMusicVolume(float volume);
    static void setSoundVolume(float volume);
    static float getMasterVolume();
    static float getMusicVolume();
    static float getSoundVolume();

private:
    static float masterVolume;
    static float musicVolume;
    static float soundVolume;
    static std::shared_ptr<CppMusic> currentMusic;
    static std::unordered_map<std::string, std::shared_ptr<CppMusic>> musics;
    static std::unordered_map<std::string, Sound> sounds;
};

