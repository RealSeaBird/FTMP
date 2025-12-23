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

using namespace std;
using namespace ftxui;

auto screen = ScreenInteractive::Fullscreen();




int main(){

  auto renderer = Renderer([&]{
    auto test = vbox({
          text("FTMP") | bold






        });






    return test;

  });


  screen.Loop(renderer);
  return 0;
}
