#!/usr/bin/python3

import os

def escape_strings(xml: str) -> str:
    return xml.replace('"', '\\"')

def stringify(xml: str) -> str:
    result = ''
    for line in xml.split('\n'):
        if line.strip() == '':
            continue
        result += '"' + line + '\\n"'
        result += '\n'
    result = result.rstrip()
    result += ';'

    return result

files = os.listdir('./introspections')

header = ''
header += '#ifndef _DESKTOPD_INTROSPECTIONS_H\n'
header += '#define _DESKTOPD_INTROSPECTIONS_H\n'
header += '\n'


for filename in files:
    const_name = filename.replace('.xml', '')

    header += 'const char *const ' + const_name + ' =\n'
    header += '    DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE\n'

    f = open('introspections/' + filename, 'r')
    text = f.read()
    f.close()

    xml_c_str = escape_strings(text)
    xml_c_str = stringify(xml_c_str)

    for line in xml_c_str.split('\n'):
        header += '    ' + line
        header += '\n'

    header += '\n\n'

header += '#endif /* _DESKTOPD_INTROSPECTIONS_H */\n'

f = open('src/introspections.h', 'w')
f.write(header)
f.close()
