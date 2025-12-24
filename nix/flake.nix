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
          
          # FTXUI - TUI library
          ftxui
          
          # SDL2 and related libraries for audio
          SDL2
          SDL2_sound
          SDL2_mixer
          SDL2_mixer.dev
          
          # Additional build tools
          ninja
          gdb
        ];
        
        # Ensure pkg-config can find SDL2 libraries
        shellHook = ''
          export PKG_CONFIG_PATH="${pkgs.SDL2}/lib/pkgconfig:${pkgs.SDL2_mixer}/lib/pkgconfig:$PKG_CONFIG_PATH"
          export LD_LIBRARY_PATH="${pkgs.SDL2}/lib:${pkgs.SDL2_mixer}/lib:$LD_LIBRARY_PATH"
          echo "FTMP development environment loaded"
        '';
      };
    }
  );
}
