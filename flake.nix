{
  description = "";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:

    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        buildPackages = with pkgs; [
          dpp
          yt-dlp-light
          ffmpeg
          # will also need openssl and zlib for runtime
        ];
      in
      with pkgs;
      {
        devShells.default = mkShell {
          buildInputs = [
            clang-tools
            clang
            cmake
            openssl.dev
            zlib.dev
          ]
          ++ buildPackages;

          shellHook = ''
            export PROJECT_PREFIX=marigold
            echo Entered dev shell for project marigold
          '';
        };

      }
    );
}
