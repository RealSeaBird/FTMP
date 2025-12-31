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

// The important stuff idk anymore

auto screen = ScreenInteractive::Fullscreen();

const char* home = std::getenv("HOME");

// strings

std::string music_dir;



string file_location;
string current_song;

// Vectors

vector<string> playlists = {
  "stuff",
    "more stuff",
    "this is just a test"



};

vector<string> songs;

// Component Indexes

int playlist_selcted = 0;
int song_selected = 0;
int volume = 60;


// Functions

std::string song_from_index(const std::vector<std::string>& songs,
                            int index) {
  if (index < 0 || index >= static_cast<int>(songs.size())) {
    return ""; // or throw, or return optional
  }
  return songs[index];
}




std::vector<std::string> get_songs() {
  std::vector<std::string> files;
  std::string docs_path =
    file_location.empty() ? music_dir : file_location;

  std::cout << "Scanning: " << docs_path << '\n';

  auto ends_with = [](const std::string& s,
                      const std::string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(),
                     suffix.size(),
                     suffix) == 0;
  };

  if (docs_path.empty() || !std::filesystem::exists(docs_path)) {
    return files;
  }

  try {
    for (const auto& entry :
         std::filesystem::directory_iterator(docs_path)) {

      if (!entry.is_regular_file())
        continue;

      std::string filename =
        entry.path().filename().string();

      if (ends_with(filename, ".mp3") ||
          ends_with(filename, ".wav") ||
          ends_with(filename, ".ogg")) {

        files.push_back(filename);
          }
         }
  } catch (const std::filesystem::filesystem_error& e) {
    std::cerr << e.what() << '\n';
  }

  return files;
}

auto play_resume()
{



}






int main(){
  // Stuff Declaration
  if (home) {
    music_dir = std::string(home) + "/Music";
  }
  vector<string> songs_in_music_dir = get_songs();

  MenuOption songs_menu_option;

  Mix_Music* music = nullptr;



  //SDL stuff goes here

  SDL_Init(SDL_INIT_AUDIO);
  Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

  int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
  int initted = Mix_Init(flags);

  if ((initted & flags) != flags) {
    std::cerr << "Mix_Init failed: " << Mix_GetError() << "\n";
  }



  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
    return 1;
  }


  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
    return 1;
  }





  current_song = song_from_index(songs_in_music_dir, song_selected);

  //Magic number dont touch
  bool has_it_done_it_next = false;
  bool has_it_done_it_last = false;




  songs_menu_option.on_change = [&]()
  {
    current_song = song_from_index(songs_in_music_dir, song_selected);
    auto song_to_play = string(home) + "/Music/" + current_song;
    if (music) {
      Mix_HaltMusic();
      Mix_FreeMusic(music);
    }

    music = Mix_LoadMUS(song_to_play.c_str());
    if (!music) {
      std::cerr << "Failed to load music: " << Mix_GetError() << "\n";
    }
  };








  //Component Definition
  auto playlists_menu = Radiobox(&playlists, &playlist_selcted);
  auto songs_menu = Menu(&songs_in_music_dir, &song_selected, songs_menu_option );
  auto play_button = Button("▶ Play", [&]
  {
    if (Mix_PlayingMusic() == 1)
    {
      Mix_ResumeMusic();
    }
    else if (Mix_PlayingMusic() == 0)
    {
      Mix_PlayMusic(music, 1);
    }
  });
  auto pause_button = Button("⏸ Pause", [&]{ Mix_PauseMusic(); });
  auto next_button = Button("⏭ Next", [&]
  {
    if (has_it_done_it_next == true)
    {
      has_it_done_it_next = false;
      current_song = song_from_index(songs_in_music_dir, song_selected);
     auto song_to_play = string(home) + "/Music/" + current_song;
     if (music) {
       Mix_HaltMusic();
       Mix_FreeMusic(music);
     }

     music = Mix_LoadMUS(song_to_play.c_str());
     if (!music) {
       std::cerr << "Failed to load music: " << Mix_GetError() << "\n";
     }
      Mix_PlayMusic(music, 1);
    }

    else if (has_it_done_it_next == false)
    {
      has_it_done_it_next = true;
      song_selected++;



    }

  });
  auto prev_button = Button("⏮ Previous", [&]
  {
    if (has_it_done_it_last == true)
    {
      has_it_done_it_last = false;
      current_song = song_from_index(songs_in_music_dir, song_selected);
     auto song_to_play = string(home) + "/Music/" + current_song;
     if (music) {
       Mix_HaltMusic();
       Mix_FreeMusic(music);
     }

     music = Mix_LoadMUS(song_to_play.c_str());
     if (!music) {
       std::cerr << "Failed to load music: " << Mix_GetError() << "\n";
     }
      Mix_PlayMusic(music, 1);
    }

    else if (has_it_done_it_last == false)
    {
      has_it_done_it_last = true;
      song_selected--;



    }


  });
  auto volume_slider = Slider("Volume:", &volume, 0, 100, 1);

  // Container
  auto container = Container::Vertical({
    Container::Horizontal(Components{playlists_menu, songs_menu}),
    Container::Horizontal(Components{play_button, pause_button}),
    Container::Horizontal(Components{next_button, prev_button}),
    Container::Horizontal(Components{volume_slider})
  });










  // Rendering

  auto renderer = Renderer(container,[&]{
    auto menu_playlist = playlists_menu-> Render();
    auto menu_song = songs_menu-> Render();
    auto button_play = play_button-> Render();
    auto button_pause = pause_button-> Render();
    auto button_next = next_button-> Render();
    auto button_prev = prev_button-> Render();
    auto slider_volume = volume_slider-> Render();


    // V/H boxes

    auto song_playing = vbox({
          text("Currently playing: " + current_song) | bold | border








        });




    // Returning the stuff

    return vbox({
      song_playing,
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

  // Loops renderer
  screen.Loop(renderer);
  // Returns 0 (duh)
  return 0;
}
