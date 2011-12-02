#!/usr/bin/python
import sys

FILTER=''.join([(len(repr(chr(x)))==3) and chr(x) or '.' for x in range(256)])

def dump(src, length, name):
	N = 0
	result='\tstatic Uint8 %s[%i] =\n\t{\n' % (name, len(src))
	while src:
		s,src = src[:length],src[length:]
		hexa = ' '.join(["0x%02X,"%ord(x) for x in s])
		s = s.translate(FILTER)
		result += "\t\t%-*s\n" % (length*3, hexa)
		N+=length

	result += '\t};\n'
	return result

print '/****************************************************************************'
print ' *            resourcedata.hpp'
print ' *'
print ' * Author: 2010  Daniel Jungmann <dsj@gmx.net>'
print ' * Copyright: See COPYING file that comes with this distribution'
print ' ****************************************************************************/'
print ''
print '#ifndef	_RESOURCEDATA_HPP_'
print '#define	_RESOURCEDATA_HPP_'
print ''
print '#ifndef	__cplusplus'
print '#error	"Including C++ header in C translation unit!"'
print '#endif	/* __cplusplus */'
print ''
print '#include "prerequisites.hpp"'
print ''
print 'namespace eternal_lands'
print '{'
print ''

for i in range(0, 15):
	pngstring = open('test%i.dds' % i, "rb").read()
	print dump(pngstring, 8, 'resource_%i' % i)

print '\tconst Uint32 resource_data_size[15] = {'

for i in range(0, 15):
	pngstring = open('test%i.dds' % i, "rb").read()
	print '\t\t%i,' % len(pngstring)
print '\t};'
print ''

print '\tconst Uint8* resource_datas[15] = {'

for i in range(0, 15):
	print '\t\tresource_%i,' % i
print '\t};'
print ''
print '}'
print ''
print '#endif	/* _RESOURCEDATA_HPP_ */'
print ''
