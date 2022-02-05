from ctypes import *


fd = CDLL('./libpprint.so')
say_my_name = fd.pprint
say_my_name.argtype = c_char_p


say_my_name('Иван!'.encode('utf-8'))

