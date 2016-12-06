#!/usr/bin/python

import Image
img = Image.open("font.png")

f = open("font.h", "w")

f.write("""#pragma once
enum {
	CHAR_WIDTH	= %d,
	CHAR_HEIGHT	= %d,
};\n""" % (img.width / 16, img.height / 16))

f.write("static const uint8_t font[] = {\n")
for y in range(img.height):
	l = ""
	for x in range(0, img.width, 8):
		b = sum((img.getpixel((x + i, y))[0] > 0) << i for i in range(8))
		l +=" %d," % b
	f.write("\t" + l[1:] + "\n")
f.write("};\n")
