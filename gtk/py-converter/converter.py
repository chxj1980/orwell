#!/usr/bin/env python3
#Converts things from here: https://github.com/yuan631111976/qtavplayer/blob/b4394d620076cedb05fdc006daee2918baeb73d9/AVOutput.cpp
import re

def constructorWith14Elements(elements):
    struct = '''pixelFormats[{0}] = {{
    {{{1},{2},{3}}},  //AVRational yuvSizes[3];
    {{{4},{5},{6}}},  //AVRational yuvWidths[3];
    {{{7},{8},{9}}},  //AVRational yuvHeights[3];
    {{{10},{10},{10}}}, //GLint yuvInternalFormat[3];
    {{{11},{11},{11}}}, //GLenum yuvGlFormat[3];
    textureFormat={12},
    isPlanar={13},
    dataType=GL_UNSIGNED_BYTE
}}'''.format(*elements)
    print(struct)

def constructorWith15Elements(elements):
    struct = '''pixelFormats[{0}] = {{
    {{{1},{2},{3}}},  //AVRational yuvSizes[3];
    {{{4},{5},{6}}},  //AVRational yuvWidths[3];
    {{{7},{8},{9}}},  //AVRational yuvHeights[3];
    {{{10},{10},{10}}}, //GLint yuvInternalFormat[3];
    {{{11},{11},{11}}}, //GLenum yuvGlFormat[3];
    textureFormat={12},
    isPlanar={13},
    dataType={14}
}}'''.format(*elements)
    print(struct)

def constructorWith18Elements(elements):
    struct = '''pixelFormats[{0}] = {{
    {{{1},{2},{3}}},  //AVRational yuvSizes[3];
    {{{4},{5},{6}}},  //AVRational yuvWidths[3];
    {{{7},{8},{9}}},  //AVRational yuvHeights[3];
    {{{10},{12},{14}}}, //GLint yuvInternalFormat[3];
    {{{11},{13},{15}}}, //GLenum yuvGlFormat[3];
    textureFormat={16},
    isPlanar={17},
    dataType=GL_UNSIGNED_BYTE
}}'''.format(*elements)
    print(struct)


with open('formats.cpp', 'r') as file:
    data = file.read()
data = data.splitlines()
data = [x for x in data if x!='']
data = [x for x in data if not '//' in x]
data = list(map(lambda x: x.replace(';', ''), data))
data = list(map(lambda x: x.replace(' ', ''), data))

#print(data[0])
for d in data:
    pixel_format = re.search('params\[(.+)\]', d).group(1)
    render_params = re.search('RenderParams\((.+)\)', d).group(1)
    l = render_params.split(',')
    if len(l)==14:
        constructorWith14Elements(l)
    elif len(l)==15:
        constructorWith15Elements(l)
    elif len(l)==18:
        constructorWith18Elements(l)
    else:
        print("DIFFERENT LENGTH! " + str(len(l)) + " for " + pixel_format)


#print(data)
