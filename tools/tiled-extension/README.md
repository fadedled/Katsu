# Katsu tilemap exporter for Tiled

This is a simple extension for [Tiled](https://www.mapeditor.org/), it produces a C source file were each layer is written using Katsu's tilemap format. This can be used as a base for creating more complex data.

## Installation

Tiled should be installed beforehand, place the ```katsu_exporter.js``` file in the locations mentioned [HERE](https://doc.mapeditor.org/en/stable/manual/scripting/#scripted-extensions). Loading should be automatic.

## Usage

When creating a new map, it should have the following properties:

- **Orientation:** Orthogonal
- **Tile render order:** Right Down
- **Width:** 8px
- **Height:** 8px

The top-most tile in the tileset will always have ID 0, the ID increases row first. Flipping is also exported. By default each tile uses palette 0, you can set the palettes of specified tiles by adding a custom property to selected tiles of the tileset ([as seen here](https://doc.mapeditor.org/en/stable/manual/custom-properties/#adding-properties)). The new property must be named **Palette** and should be an **int** (remember that valid palette numbers go from 0 to 127).

To export a tilemap go to **File -> Export As...** then select **Katsu tilemap C file**. The name used for the file exporter will be used for each symbol, for example, if the filename is *katsu_exporter.js*, then the output file will contain the integers *my_tilemap_w* and *my_tilemap_h* for the map's width and height, respectively. For each layer's data the symbols will have the format `<filename>_<Layer Number>_data`, for example, for a two layer map the symbols would be *my_tilemap_0_data* and *my_tilemap_1_data*.



