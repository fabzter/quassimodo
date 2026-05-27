{
  description = "Quassimodo isolated build environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";

  outputs = { self, nixpkgs }:
    let
      systems = [ "aarch64-darwin" "x86_64-darwin" "x86_64-linux" "aarch64-linux" ];
      forAll = f: nixpkgs.lib.genAttrs systems (s: f (import nixpkgs { system = s; }));
    in {
      packages = forAll (pkgs: {
        boostPython = pkgs.boost.override {
          enablePython = true;
          python = pkgs.python311;
        };
      });

      devShells = forAll (pkgs: {
        default = pkgs.mkShell {
          packages = [
            pkgs.cmake
            pkgs.ninja
            pkgs.pkg-config
            pkgs.python311
            (pkgs.boost.override { enablePython = true; python = pkgs.python311; })
          ];
        };
      });
    };
}
