# Minetest's Irrlicht fork (IrrlichtMt) built standalone for darwin/arm64.
# Stock nixpkgs `irrlicht` is BROKEN on darwin; this fork builds on macOS and is
# the same engine Luanti (Minetest) ships with on darwin/arm64.
#
# Source of truth: nixpkgs pkgs/games/minetest/default.nix (the Luanti package)
# which builds this fork on darwin using the Cocoa / OpenGL / IOKit frameworks.
# The fork's own source/Irrlicht/CMakeLists.txt does:
#   find_library(COCOA_LIB Cocoa); find_library(IOKIT_LIB IOKit);
#   find_package(OpenGL) -> so those three frameworks are exactly what is needed.
{ lib, stdenv, fetchFromGitHub, cmake, zlib, libjpeg, libpng
, libGL, libX11, darwin ? null }:

stdenv.mkDerivation rec {
  pname = "irrlichtmt";
  version = "1.9.0mt15";

  src = fetchFromGitHub {
    owner = "minetest";
    repo = "irrlicht";
    rev = version;
    hash = "sha256-2zqG2JWdq+1QtgbgH8W7q+3yvREgMvINYh0phQvTv0c=";
  };

  nativeBuildInputs = [ cmake ];

  # Required everywhere by source/Irrlicht/CMakeLists.txt:
  #   find_package(ZLIB/JPEG/PNG REQUIRED) for the image loaders/writers.
  buildInputs = [ zlib libjpeg libpng ]
    ++ lib.optionals stdenv.hostPlatform.isLinux [ libGL libX11 ]
    ++ lib.optionals stdenv.hostPlatform.isDarwin
       (with darwin.apple_sdk.frameworks; [ Cocoa OpenGL IOKit ]);

  # Upstream defaults BUILD_SHARED_LIBS=ON, which is what Luanti links against.
  # Tests/examples are not needed for downstream consumers and only add failure
  # surface, so disable them.
  cmakeFlags = [
    "-DBUILD_EXAMPLES=OFF"
  ];

  meta = {
    description = "Minetest's (Luanti's) Irrlicht 3D engine fork";
    homepage = "https://github.com/minetest/irrlicht";
    license = lib.licenses.zlib;
    platforms = lib.platforms.linux ++ lib.platforms.darwin;
  };
}
