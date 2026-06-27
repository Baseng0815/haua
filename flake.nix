{
  description = "haua — home automation unified architecture";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default = pkgs.mkShell.override
          {
            stdenv = pkgs.clangStdenv;
          }
          {
            name = "haua-dev";

            nativeBuildInputs = with pkgs; [
              clang-tools # clangd, clang-format, clang-tidy
              cmake
              ninja
              pkg-config
              python3 # open62541's code generation
              git
            ];

            buildInputs = with pkgs; [
              mbedtls
            ];

            shellHook = ''
              echo "haua dev shell — $(clang --version | head -1)"
              zsh
            '';
          };
      });
}
