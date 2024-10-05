/* 
 * Katsu Tuled Exporter Extension 1.0 - public domain
 * katsu_tiled_exporter.js 
 */

function toHex(num) {
    return "0x" + num.toString(16).padStart(2, '0').toUpperCase();
}


var customMapFormat = {
    name: "Katsu tilemap C file",
    extension: "c",

    write: function(map, fileName) {

        var file = new TextFile(fileName, TextFile.WriteOnly);
        map_name = fileName.replace(/^.*[\\/]/, '').split('.')[0]; 
        file.write("const unsigned int " + map_name + "_w = " + map.width + ";\n");
        file.write("const unsigned int " + map_name + "_h = " + map.height + ";\n\n");
        
        for (var i = 0; i < map.layerCount; ++i) {
            var layer = map.layerAt(i);
            if (layer.isTileLayer) {
                var rows = [];
                var bytes = map.width * map.height * 4;
                file.write("const unsigned char " + map_name + "_" + i + "_data[" + bytes + "] = {\n\t");
                for (y = 0; y < layer.height; ++y) {
                    var row = [];
                    for (x = 0; x < layer.width; ++x) {
                        var tile = layer.cellAt(x, y);
                        if ( layer.tileAt(x, y) != null) {
                            var pal = layer.tileAt(x, y).properties()["Palette"];
                            if (typeof pal == 'number') {
                                pal = pal & 0x7F;
                            } else {
                                pal = 0; 
                            }                        
                        }
                    
                        var fthi = (tile.tileId & 0x3F00) >> 8;
                        var tlo = (tile.tileId & 0xFF);
                        if (tile.flippedHorizontally) {
                            fthi |= 0x40;
                        }
                        if (tile.flippedVertically) {
                            fthi |= 0x80;
                        }
                        
                        file.write( toHex(pal) + ", 0x00, " + toHex(fthi) + ", " + toHex(tlo) + ", ");
                    }
                    file.write("\n\t");
                }
                file.write("};\n\n");
            }
        }

        file.commit();
    },
}

tiled.registerMapFormat("custom", customMapFormat)

