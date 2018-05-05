from PIL import Image


def monochrome(img_name):
    image = Image.open(img_name)
    image = image.convert('1')
    return image


def read_pixels(image, x, y):
    pixels = image.load()
    return pixels[x, y]


def convert(image):
    name = image
    img = monochrome(name)
    width, heigth = img.size
    x = 0
    y = 0
    chars = {0: "$", 255: " "}
    file = open(name.split(".")[0] + ".txt", 'w')
    while y < heigth-1:
        rgb = read_pixels(img, x, y)
        file.write(chars[rgb])
        x += 1
        if x == width -1:
            file.write('\n')
            x = 0
            y += 1
    file.close()
