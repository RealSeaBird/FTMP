#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <random>
#include <ctime>
#include <stdbool.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace std;
using namespace ftxui;

auto screen = ScreenInteractive::Fullscreen();







vector<string> playlists = {
  "stuff",
    "more stuff",
    "this is just a test"



};

vector<string> songs = {
  "Magical cure loveshot",
  "kyu-kuratin",
  "TRAFFIK!"


};



int playlist_selcted = 0;
int song_selected = 0;
int volume = 60;
string example_song = "Magical cure loveshot";




int main(){

  auto playlists_menu = Radiobox(&playlists, &playlist_selcted);
  auto songs_menu = Radiobox(&songs, &song_selected);
  auto play_button = Button("▶ Play", [&]{ /* call your play function */ });
  auto pause_button = Button("⏸ Pause", [&]{ /* call your pause function */ });
  auto next_button = Button("⏭ Next", [&]{ /* call your next function */ });
  auto prev_button = Button("⏮ Previous", [&]{ /* call your previous function */ });
  auto volume_slider = Slider("Volume:", &volume, 0, 100, 1);

  auto container = Container::Vertical({
    Container::Horizontal(Components{playlists_menu}),
    Container::Horizontal(Components{play_button, pause_button}),
    Container::Horizontal(Components{next_button, prev_button}),
    Container::Horizontal(Components{volume_slider})
  });



  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
    return 1;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
    return 1;
  }






  auto renderer = Renderer(container,[&]{
    auto menu_playlist = playlists_menu-> Render();
    auto menu_song = songs_menu-> Render();
    auto button_play = play_button-> Render();
    auto button_pause = pause_button-> Render();
    auto button_next = next_button-> Render();
    auto button_prev = prev_button-> Render();
    auto slider_volume = volume_slider-> Render();

    auto test = vbox({
          text("Currently playing: " + example_song) | bold | border








        });






    return vbox({
      test,
      hbox ({
        menu_playlist | border | flex,
        menu_song | border | flex
      }),
      hbox({
        button_prev,
        button_play,
        button_pause,
        button_next,
        slider_volume | border


      }) | bold | border



    });

  });


  screen.Loop(renderer);
  return 0;
}
