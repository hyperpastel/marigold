{
  description = "A discord bot that downloads youtube videos as mp3s";

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
        pkgs = import nixpkgs { inherit system; };

        buildPackages = with pkgs; [
          dpp
          openssl
          zlib
        ];

        cmakeBuild = pkgs.stdenv.mkDerivation {
          name = "marigold";
          version = "1.0.0";

          src = ./.;

          nativeBuildInputs = [
            pkgs.cmake
            pkgs.makeWrapper
            pkgs.pkg-config
          ];

          buildInputs = buildPackages ++ [
            pkgs.ffmpeg
            pkgs.yt-dlp-light
          ];

          # We don't have tests (yet?)
          doCheck = false;

          installPhase = ''
            mkdir -p $out/bin
            cp marigold $out/bin
          '';

          postFixup = ''
            find
             wrapProgram $out/bin/marigold --set PATH "${
               pkgs.lib.makeBinPath [
                 pkgs.yt-dlp-light
                 pkgs.ffmpeg
               ]
             }"
          '';

        };
      in
      with pkgs;
      {
        devShells.default = mkShell {
          buildInputs = [
            ffmpeg
            yt-dlp-light
            clang-tools
            cmake
          ]
          ++ buildPackages;

          shellHook = ''
            export PROJECT_PREFIX=marigold
            echo "Entered dev shell for project marigold"
          '';
        };

        packages = {
          default = cmakeBuild;
          marigold = cmakeBuild;
        };

      }
    );
}
