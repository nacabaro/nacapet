from PIL import Image


def load_image(filename: str) -> Image.Image:
    image = Image.open(filename)
    image = image.convert("RGB")
    return image


def convert_img_to_rgb565(image: Image.Image) -> bytearray: 
    width, height = image.size
    
    rgb565_data = bytearray()

    for y in range(height):
        for x in range(width):
            r, g, b = image.getpixel((x, y))

            r = (r >> 3) & 0x1F
            g = (g >> 2) & 0x3F 
            b = (b >> 3) & 0x1F
            
            high_byte = (r << 3) | (g >> 3)
            low_byte = ((g << 5) | b) & 0xFF
            

            rgb565_data.append(high_byte)
            rgb565_data.append(low_byte)
            
    return width, height, rgb565_data


def save_rgb565(filename: str, rgb565_data: list[bytearray], width: int, height: int):
    with open(filename, "wb") as f:
        ## A lo mejor cambio esta cabecera? ns
        f.write(width.to_bytes(1, "little"))
        f.write(height.to_bytes(1, "little"))
        f.write(rgb565_data)
        
        f.close()
        
        
if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python bg_to_rgb565.py <input_image> <output_file>")
        sys.exit(1)

    input_image = sys.argv[1]
    output_file = sys.argv[2]

    image = load_image(input_image)
    width, height, rgb565_data = convert_img_to_rgb565(image)
    save_rgb565(output_file, rgb565_data, width, height)