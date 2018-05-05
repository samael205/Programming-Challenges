from PIL import Image
import imghdr

validImageFormat = ["png", "bmp", "jpeg", "jpg"]


def changeExtension(imageName, newExtension):
  	if imghdr.what(imageName) == newExtension:
  		print('\033[33msame image format! ' + newExtension)		
	elif newExtension in validImageFormat and imghdr.what(imageName):
   		image = Image.open(imageName)
   		print('\033[34mConvert ' + imageName + ' to ' + newExtension)
  		image.save(imageName[:imageName.rfind(".")] + "." + newExtension)


