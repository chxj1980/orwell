#!/usr/bin/env python3
#Converts things from here: https://github.com/yuan631111976/qtavplayer/blob/b4394d620076cedb05fdc006daee2918baeb73d9/AVOutput.cpp
import re

def constructorWith14Elements(elements):
    struct = '''pixelFormats[AV_PIX_FMT_YUV420P10LE] = {{
    //f1,f2,f3
    //GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE
    {{{0},{1},{2}}},  //AVRational yuvSizes[3];
    {{{3},{4},{5}}},  //AVRational yuvWidths[3];
    {{{6},{7},{8}}},  //AVRational yuvHeights[3];
    {{GL_LUMINANCE_ALPHA, }},//GLint yuvInternalFormat[3];
    {{ }},             //GLenum yuvGlFormat[3];

    }}'''.format(elements)

with open('formats.cpp', 'r') as file:
    data = file.read()
data = data.splitlines()
data = [x for x in data if x!='']
data = [x for x in data if not '//' in x]
data = list(map(lambda x: x.replace(';', ''), data))
data = list(map(lambda x: x.replace(' ', ''), data))

print(data[0])
pixel_format = re.search('params\[(.+)\]', data[0]).group(1)
render_params = re.search('RenderParams\((.+)\)', data[0]).group(1)
l = render_params.split(',')
if len(l)==14:
    constructorWith14Elements(l)


#print(data)
