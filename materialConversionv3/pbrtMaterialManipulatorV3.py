import re
from dataclasses import dataclass

FILE_NAME = "material-testball/scene-v3.pbrt"
FILE_NAME_MATERIALS = "materials.mat"
PATTERN_BEGIN = "MakeNamedMaterial \"Material\""
PATTERN_END = "MakeNamedMaterial"

MATERIAL_PLASTIC = 'MAT-plastic'
MATERIAL_SHINY = 'MAT-shiny'
MATERIAL_MATTE = 'MAT-matte'
MATERIAL_GLASS = 'MAT-glass'
MATERIAL_SUBSTRATE = 'MAT-substrate'
MATERIAL_UBER = 'MAT-uber'
MATERIAL_TRANSLUCENT = 'MAT-translucent'

@dataclass
class Vector3():
    x: float = 0
    y: float = 0
    z: float = 0

@dataclass
class Material():
    name: str = ""
    type: str = ""
    eta: float = 0
    roughnness: float = 0
    diffuse: Vector3 = Vector3()
    specular: Vector3 = Vector3()
    transmittance: Vector3 = Vector3()


def read_material(material_text):
    words = material_text.split(" ")
    m = Material()
    m.name = words[0]
    m.type = words[4]
    index = 5
    print(words)
    while index < len(words):
        if words[index] == 'r':
            index += 2
            m.roughnness = float(words[index])
            index += 2
        elif words[index] == 'Kd':
            index += 2
            m.diffuse = Vector3(float(words[index]), float(words[index+1]), float(words[index+2]))
            index += 4
        elif words[index] == 'Ks':
            index += 2
            m.specular = Vector3(float(words[index]), float(words[index+1]), float(words[index+2]))
            index += 4
            
    # if m.type == MATERIAL_DIFFUSE:
    #     m.reflectance = Vector3(float(words[7]), float(words[8]), float(words[9]))
    # if m.type == MATERIAL_DIELECTRIC:
    #     m.eta = float(words[7])
    # if m.type == MATERIAL_DIFFUSE_TRANSMISSION:
    #     m.reflectance = Vector3(float(words[7]), float(words[8]), float(words[9]))
    #     m.transmittance = Vector3(float(words[13]), float(words[14]), float(words[15]))
    # if m.type == MATERIAL_CONDUCTOR:
    #     m.roughnness = float(words[7])
    #     m.reflectance = Vector3(float(words[11]), float(words[12]), float(words[13]))
    return m

def add_new_material(index):
    output_lines = []
    try:
        with open(FILE_NAME_MATERIALS, 'r') as file_in:
            lines = file_in.readlines()
    except FileNotFoundError:
        print("File not found")
    
    m = read_material(lines[index])
    if m.type == MATERIAL_PLASTIC:
        output_lines.append('    "string type" [ "plastic" ]\n')
        output_lines.append(f'    "float roughness" [ {m.roughnness} ]\n')
        output_lines.append(f'    "color Kd" [ {m.diffuse.x} {m.diffuse.y} {m.diffuse.z} ]\n')
        output_lines.append(f'    "color Ks" [ {m.specular.x} {m.specular.y} {m.specular.z} ]\n')
    # if m.type == MATERIAL_DIFFUSE:
    #     output_lines.append('    "string type" [ "diffuse" ]\n')
    #     output_lines.append(f'    "rgb reflectance" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
    # if m.type == MATERIAL_DIELECTRIC:
    #     output_lines.append('    "string type" [ "dielectric" ]\n')
    #     output_lines.append(f'    "float roughness" [ {m.eta} ]\n')
    # if m.type == MATERIAL_DIFFUSE_TRANSMISSION:
    #     output_lines.append('    "string type" [ "diffusetransmission" ]\n')
    #     output_lines.append(f'    "rgb reflectance" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
    #     output_lines.append(f'    "rgb transmittance" [ {m.transmittance.x} {m.transmittance.y} {m.transmittance.z} ]\n')
    # if m.type == MATERIAL_CONDUCTOR:
    #     output_lines.append('    "string type" [ "conductor" ]\n')
    #     output_lines.append(f'    "float roughness" [ {m.roughnness} ]\n')
    #     output_lines.append(f'    "rgb reflectance" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
    
    
    return output_lines, m

def replace_Mat(index):
    try:
        with open(FILE_NAME, 'r') as file_in:
            lines = file_in.readlines()
    except FileNotFoundError:
        print("File not found")
        exit(1)
    
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
        
        if re.search(f'material-testball', line):
            output_lines.append(f'    \"string filename\" [ \"matBallsOutput/material-testball{index}.png\" ]\n')
            continue
        output_lines.append(line)
        
        if re.search(PATTERN_BEGIN, line):
            marker_found = True
            
    with open(f'material-testball/{m.name}_{index}.pbrt', 'w') as file:
        for string in output_lines:
            file.write(string)
    

if __name__ == '__main__':
    for i in range(1):
        replace_Mat(i)