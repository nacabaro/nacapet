from PIL import Image
import os
import sys

def create_new_canvas(width: int, height: int):
    new_canvas = Image.new("RGBA", (width, height), color=(0, 0, 0, 0))
    return new_canvas


def get_image_files(dir: str):
    files = os.listdir(dir)
    image_files = [ file for file in files if file.endswith("png") ]
    return image_files


def check_correct_amount_of_images(image_files: list[str]):
    return len(image_files) == 12


def reduce_image(file_name: str):
    image = Image.open(file_name)
    result_image = Image.new("RGBA", (16, 16), color=(0, 0, 0, 0))
    
    width, height = image.size
        
    if 48 not in (width, height):
        print("Invalid again")
        return None
    
    for x in range(width // 3):
        for y in range(height // 3):
            pixel = image.getpixel((x * 3, y * 3))
            result_image.putpixel((x, y), pixel)
            
    return result_image

def append_image(image, canvas, n):
    width, height = canvas.size    

    n_tiles_x = width // 16

    x = (n % n_tiles_x) * 16
    y = (n // n_tiles_x) * 16

    canvas.paste(image, (x, y))
    
    return canvas
    

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("invalido")
        sys.exit(-1)

    new_canvas = create_new_canvas(48, 64)
    image_files = get_image_files(sys.argv[1])
    if not check_correct_amount_of_images(image_files):
        sys.exit(-1)
    
    resulting_images = []
    
    for i in range(12):
        resulting_image = reduce_image(f'{sys.argv[1]}/{i}.png')
        new_canvas = append_image(resulting_image, new_canvas, i)
        
        
    new_canvas.save(sys.argv[2])

    print("Sprite sheet creada!!")
    
    exit(0)