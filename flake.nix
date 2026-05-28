{
  description = "Quassimodo isolated build environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";

  outputs = { self, nixpkgs }:
    let
      systems = [ "aarch64-darwin" "x86_64-darwin" "x86_64-linux" "aarch64-linux" ];
      forAll = f: nixpkgs.lib.genAttrs systems (s: f (import nixpkgs { system = s; }));
    in {
      packages = forAll (pkgs:
        let
          irrlichtFork = pkgs.callPackage ./nix/irrlicht-fork.nix { darwin = pkgs.darwin; };
        in { inherit irrlichtFork; });

      devShells = forAll (pkgs:
        let
          irrlichtFork = pkgs.callPackage ./nix/irrlicht-fork.nix { darwin = pkgs.darwin; };
        in {
          default = pkgs.mkShell {
            packages = [ pkgs.cmake pkgs.ninja pkgs.pkg-config pkgs.python311
                         pkgs.python311.pkgs.pybind11 irrlichtFork ];
          };
        });
    };
}
