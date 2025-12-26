{
  description = "Nix flake for the FTMP music player";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShells.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          gcc
          cmake
          pkg-config
          ninja
          gdb

          # TUI
          ftxui

          # SDL
          SDL2
          SDL2_mixer

          # Audio codecs
          mpg123
          libogg
          libvorbis
        ];

        shellHook = ''
          echo "FTMP dev shell (SDL2_mixer + MP3 enabled)"
        '';
      };
    });
}
