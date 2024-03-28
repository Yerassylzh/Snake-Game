from PIL import Image


im = Image.open("fieldBackground.png")
pixels = im.load()
width, height = im.size 
print(pixels[10, 10])
print(pixels[30, 30])