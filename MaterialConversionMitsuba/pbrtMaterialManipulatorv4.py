import re
from dataclasses import dataclass

FILE_NAME = "material-testball/scene_v0.6.xml"
FILE_NAME_MATERIALS = "materials.mat"
PATTERN_BEGIN = "<bsdf type=\"twosided\" id=\"Material\" >"
PATTERN_END = "</bsdf>"

MATERIAL_COATED_DIFFUSE = 'MAT-coateddiffuse'
MATERIAL_DIFFUSE = 'MAT-diffuse'
MATERIAL_DIELECTRIC = 'MAT-dielectric'
MATERIAL_DIFFUSE_TRANSMISSION = 'MAT-diffusetransmission'
MATERIAL_CONDUCTOR = 'MAT-conductor'
MATERIAL_CONDUCTOR_REFLECTANCE = 'MAT-conductorreflectance'
MATERIAL_COATED_CONDUCTOR = 'MAT-coatedconductor'

@dataclass
class Vector3():
    x: float
    y: float
    z: float

@dataclass
class Material():
    name: str
    type: str
    eta: float
    roughnness: float
    reflectance: Vector3
    transmittance: Vector3
    eta_conductor: str
    k_conductor: str


def read_material(material_text):
    words = material_text.split(" ")
    m = Material("", "", 0, 0, Vector3(0,0,0), Vector3(0,0,0), "", "")
    m.name = words[0]
    m.type = words[4]
    print(words)
    if m.type == MATERIAL_COATED_DIFFUSE:
        m.roughnness = words[7]
        m.reflectance = Vector3(float(words[11]), float(words[12]), float(words[13]))
    if m.type == MATERIAL_DIFFUSE:
        m.reflectance = Vector3(float(words[7]), float(words[8]), float(words[9]))
    if m.type == MATERIAL_DIELECTRIC:
        m.eta = words[7]
    if m.type == MATERIAL_DIFFUSE_TRANSMISSION:
        m.reflectance = Vector3(float(words[7]), float(words[8]), float(words[9]))
        m.transmittance = Vector3(float(words[13]), float(words[14]), float(words[15]))
    if m.type == MATERIAL_CONDUCTOR:
        m.eta_conductor = words[7]
        m.k_conductor = words[11]
        m.roughnness = words[15]
    if m.type == MATERIAL_COATED_CONDUCTOR:
        m.eta_conductor = words[7]
        m.k_conductor = words[11]
        m.roughnness = words[15]
    if m.type == MATERIAL_CONDUCTOR_REFLECTANCE:
        m.reflectance = Vector3(float(words[7]), float(words[8]), float(words[9]))
        m.roughnness = words[13]
    return m

def add_new_material(index):
    output_lines = []
    try:
        with open(FILE_NAME_MATERIALS, 'r') as file_in:
            lines = file_in.readlines()
    except FileNotFoundError:
        print("File not found")
    
    m = read_material(lines[index])
    if m.type == MATERIAL_COATED_DIFFUSE:
        output_lines.append('        <bsdf type="roughplastic" >\n')
        output_lines.append(f'            <rgb name="diffuse_reflectance" value="{m.reflectance.x} {m.reflectance.y} {m.reflectance.z}" />\n')
        output_lines.append(f'            <float name="alpha" value="{m.roughnness}" />\n')
    if m.type == MATERIAL_DIFFUSE:
        output_lines.append('        <bsdf type="diffuse" >\n')
        output_lines.append(f'            <rgb name="reflectance" value="{m.reflectance.x} {m.reflectance.y} {m.reflectance.z}" />\n')
    if m.type == MATERIAL_DIELECTRIC:
        output_lines.append('        <bsdf type="dielectric" >\n')
        output_lines.append(f'            <float name="int_ior" value="{1.1}" />\n')
        output_lines.append(f'            <float name="ext_ior" value="{1.0}"/>\n')
    if m.type == MATERIAL_DIFFUSE_TRANSMISSION:
        output_lines.append('        <bsdf type="roughdielectric" >\n')
        output_lines.append(f'            <rgb name="alpha" value="0.5" />\n')
    if m.type == MATERIAL_CONDUCTOR:
        meltal_string = m.eta_conductor.split('-')[1]
        output_lines.append('        <bsdf type="roughconductor" >\n')
        m.roughnness = min(1, float(m.roughnness)*4)
        output_lines.append(f'            <float name="alpha" value="{(m.roughnness)}" />\n')
        output_lines.append(f'            <string name="material" value="{meltal_string}"/>\n')
    if m.type == MATERIAL_COATED_CONDUCTOR:
        meltal_string = m.eta_conductor.split('-')[1]
        output_lines.append('        <bsdf type="roughconductor" >\n')
        m.roughnness = min(1, float(m.roughnness)*4)
        output_lines.append(f'            <float name="alpha" value="{m.roughnness}" />\n')
        output_lines.append(f'            <string name="material" value="{meltal_string}"/>\n')
    if m.type == MATERIAL_CONDUCTOR_REFLECTANCE:
        output_lines.append('        <bsdf type="plastic" >\n')
        output_lines.append(f'            <rgb name="diffuse_reflectance" value="{m.reflectance.x}, {m.reflectance.y}, {m.reflectance.z}"/>\n')

    
    
    return output_lines, m

def replace_Mat(index):
    try:
        with open(FILE_NAME, 'r') as file_in:
            lines = file_in.readlines()
    except FileNotFoundError:
        print("File not found")
    
    marker_found = False
    output_lines = []
    
    for line in lines:
        
        if marker_found == True:
            if re.search(PATTERN_END, line):
                marker_found = False
                new_lines, m = add_new_material(index)
                for new_line in new_lines:
                    output_lines.append(new_line)
                    
                output_lines.append(line)
            
            continue
        
        if re.search(PATTERN_BEGIN, line):
            marker_found = True

    
        output_lines.append(line)
    output_lines_2 = []
    marker_found = False
    add_id = False
    for line in output_lines:
        if m.type == MATERIAL_DIELECTRIC or m.type == MATERIAL_DIFFUSE_TRANSMISSION:
            if add_id:
                output_lines_2.append(line[:-2] + 'id="Material">\n')
                add_id = False
                continue
            if re.search(PATTERN_BEGIN, line):
                marker_found = add_id = True
                continue
            if marker_found and re.search(PATTERN_END, line):
                marker_found = False
                continue
        output_lines_2.append(line)

    
    with open(f'material-testball/{m.name}.xml', 'w') as file:
        for string in output_lines_2:
            file.write(string)
    

if __name__ == '__main__':
    for i in range(53):
        replace_Mat(i)