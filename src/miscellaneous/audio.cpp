#include "audio.h"

std::shared_ptr<CppMusic> Audio::currentMusic;
std::unordered_map<std::string, std::shared_ptr<CppMusic>> Audio::musics;
std::unordered_map<std::string, Sound> Audio::sounds;
float Audio::masterVolume = 1.0f;
float Audio::musicVolume = 1.0f;
float Audio::soundVolume = 1.0f;

void Audio::init(std::string folder) {
    InitAudioDevice();
    auto list = LoadDirectoryFiles(folder.c_str());
    for (int i = 0; i < list.count; ++i) {
        if (IsFileExtension(list.paths[i], ".ogg")) {
            musics.emplace(GetFileNameWithoutExt(list.paths[i]), std::make_shared<CppMusic>(list.paths[i]));
        } else if (IsFileExtension(list.paths[i], ".wav")) {
            sounds.emplace(GetFileNameWithoutExt(list.paths[i]), LoadSound(list.paths[i]));
        }
    }
    
}

void Audio::update() {
    if (currentMusic) UpdateMusicStream(currentMusic->data);
}

void Audio::unload() {
    for (auto& music : musics) UnloadMusicStream(music.second->data);
    for (auto& sound : sounds) UnloadSound(sound.second);
    musics.clear();
    sounds.clear();
    CloseAudioDevice();
}

void Audio::setMusic(std::string ID) {
    if (currentMusic) currentMusic->stop();
    currentMusic = musics.at(ID);
    currentMusic->play();
}

void Audio::playSound(std::string ID, float volume, float pitch) {
    if (ID.empty()) return;
    SetSoundVolume(sounds[ID], volume);
    SetSoundPitch(sounds[ID], pitch);
    PlaySound(sounds[ID]);
}

void Audio::setMasterVolume(float volume) {
    SetMasterVolume(volume);
    masterVolume = volume;
}

void Audio::setMusicVolume(float volume) {
    for (auto& music : musics) SetMusicVolume(music.second->data, volume);
    musicVolume = volume;
}

void Audio::setSoundVolume(float volume) {
    for (auto& sound : sounds) SetSoundVolume(sound.second, volume);
    soundVolume = volume;
}

float Audio::getMasterVolume() {
    return masterVolume;
}

float Audio::getMusicVolume() {
    return musicVolume;
}

float Audio::getSoundVolume() {
    return soundVolume;
}

CppMusic::CppMusic(std::string filepath) {
    data = LoadMusicStream(filepath.c_str());
    this->filepath = filepath;
}

void CppMusic::play() {
    PlayMusicStream(data);
}

void CppMusic::stop() {
    StopMusicStream(data);
}

void CppMusic::pause() {
    PauseMusicStream(data);
}

void CppMusic::seek(float seconds) {
    SeekMusicStream(data, seconds);
}

void CppMusic::resume() {
    ResumeMusicStream(data);
}
