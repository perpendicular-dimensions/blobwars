# Blob Wars: Metal Blob Solid

Metal Blob Solid is the first game in the Blob Wars series made by [Parallel Realities](http://www.parallelrealities.co.uk/).
It is a 2D platform game, where you control Bob in an effort to save his world from an invasion of alien Blobs.

Since their world was invaded by an alien race, the Blobs have faced a lifetime of war.
But now they have a chance to win the war once and for all.

In Blob Wars: Metal Blob Solid, you take on the role of a fearless Blob agent, Bob.
Bob's mission is to infiltrate the various enemy bases around the Blobs' homeworld and rescue as many MIAs as possible.
But standing in his way are many vicious aliens, other Blobs who have been assimilated and the evil alien leader, Galdov.

## Building

This game uses Meson as the build system, and depends on the fmtlib, SDL2, yaml-cpp and zlib libraries.
To install these on a Debian or Debian-derived platform, run:

    sudo apt install meson libfmt-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev libyaml-cpp-dev zlib1g-dev

Once all the dependencies are installed, run:

    meson --buildtype release build
    ninja -C build

You can try out the compiled binary using this command:

    build/src/blobwars -data .

To install the game, run the following command:

    ninja -C build install
