from PIL import Image
import imghdr

valid_image_format = ["png", "bmp", "jpeg", "jpg"]


def change_extension(image_name, new_extension):
  	if imghdr.what(image_name) == new_extension:
  		print('\033[33msame image format! ' + new_extension)		
	elif new_extension in valid_image_format and imghdr.what(image_name):
   		image = Image.open(image_name)
   		print('\033[34mConvert ' + image_name + ' to ' + new_extension)
  		image.save(image_name[:image_name.rfind(".")] + "." + new_extension)


