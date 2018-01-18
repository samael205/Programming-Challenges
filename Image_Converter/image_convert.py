from PIL import Image
import imghdr

validImageFormat = ["png", "bmp", "jpeg", "jpg"]

def changeExtension(imageName, newExtension):
	if imghdr.what(imageName) and imghdr.what(imageName) != newExtension and newExtension in validImageFormat:
   		image = Image.open(imageName)
   		print('\033[34mConvert ' +imageName + ' to ' + newExtension)
  		image.save(imageName[:imageName.rfind(".")] + "." + newExtension)
  	elif imghdr.what(imageName) == newExtension:
  		print('\033[33msame image format! ' + newExtension)	


