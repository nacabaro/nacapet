from PIL import Image
import sys


def split_image(filename: str) -> list[bytearray]:
    image = Image.open(filename)
    image = image.convert("RGBA")
    
    width, height = image.size
    n_tiles_x = width // 16
    n_tiles_y = height // 16
    
    n_tiles = n_tiles_x * n_tiles_y
    
    tiles = []
    
    for i in range(n_tiles):
        x = (i % n_tiles_x) * 16
        y = (i // n_tiles_x) * 16

        w = x + 16
        h = y + 16
        
        tile = image.crop((x, y, w, h))
        tiles.append(tile)
        
        
    return tiles


def convert_image(tiles: list) -> tuple[list[bytearray], int, int]:
    width, height = tiles[0].size
    rgb565_tiles = []

    for img in tiles:
        rgb565_data = bytearray()

        for y in range(height):
            for x in range(width):
                r, g, b, a = img.getpixel((x, y))

                r = (r >> 3) & 0x1F 
                g = (g >> 2) & 0x3F
                b = (b >> 3) & 0x1F 
                
                if a < 255:
                    high_byte = 0x01
                    low_byte = 0x20
                else:
                    high_byte = (r << 3) | (g >> 3)
                    low_byte = ((g << 5) | b) & 0xFF

                rgb565_data.append(high_byte)
                rgb565_data.append(low_byte)

        rgb565_tiles.append(rgb565_data)

    return rgb565_tiles, width, height
    

def save_rgb565(filename: str, rgb565_data: list[bytearray], width: int, height: int):
    with open(filename, "wb") as f:
        ## Y esta cabecera tambien
        f.write(width.to_bytes(1, "little"))
        f.write(height.to_bytes(1, "little"))
        f.write(len(rgb565_data).to_bytes(1, "little"))
        
        for rgb565_tile in enumerate(rgb565_data):
            f.write(rgb565_tile[1])

    
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python png_to_rgb565.py <input.png> <output.rgb565>")
        sys.exit(1)
    
    filename = sys.argv[1]
    output_filename = sys.argv[2]
    
    tiles = split_image(filename)
    rgb565_data, width, height = convert_image(tiles)
    save_rgb565(output_filename, rgb565_data, width, height)
    
    print(f"Converted {filename} to {output_filename} with dimensions {width}x{height}.")
