import re
from dataclasses import dataclass

FILE_NAME = "material-testball/scene-v4.pbrt"
FILE_NAME_MATERIALS = "materials.mat"
PATTERN_BEGIN = "MakeNamedMaterial \"Material\""
PATTERN_END = "MakeNamedMaterial"
#BF_blue01 0.0902 0.07843 0.5804 MAT-coateddiffuse roughness [ 0.6 ] reflectance [ 0.0902 0.07843 0.5804 ]
    # "string type" [ "coateddiffuse" ]
    # "float vroughness" [ 0.001 ]
    # "float uroughness" [ 0.001 ]
    # "bool remaproughness" [ false ]
    # "rgb reflectance" [ 0.243117 0.059106 0.000849 ]

MATERIAL_COATED_DIFFUSE = 'MAT-coateddiffuse'
MATERIAL_DIFFUSE = 'MAT-diffuse'
MATERIAL_DIELECTRIC = 'MAT-dielectric'
MATERIAL_DIFFUSE_TRANSMISSION = 'MAT-diffusetransmission'
MATERIAL_CONDUCTOR = 'MAT-conductor'

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


def read_material(material_text):
    words = material_text.split(" ")
    m = Material("", "", 0, 0, Vector3(0,0,0), Vector3(0,0,0))
    m.name = words[0]
    m.type = words[4]
    print(words)
    if m.type == MATERIAL_COATED_DIFFUSE:
        m.roughnness = words[7]
        m.reflectance = Vector3(float(words[11]), float(words[12]), float(words[13]))
    if m.type == MATERIAL_DIFFUSE:
        m.reflectance = Vector3(float(words[7]), float(words[8]), float(words[9]))
    if m.type == MATERIAL_DIELECTRIC:
        m.eta = float(words[7])
    if m.type == MATERIAL_DIFFUSE_TRANSMISSION:
        m.reflectance = Vector3(float(words[7]), float(words[8]), float(words[9]))
        m.transmittance = Vector3(float(words[13]), float(words[14]), float(words[15]))
    if m.type == MATERIAL_CONDUCTOR:
        m.roughnness = float(words[7])
        m.reflectance = Vector3(float(words[11]), float(words[12]), float(words[13]))
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
        output_lines.append('    "string type" [ "coateddiffuse" ]\n')
        output_lines.append(f'    "float roughness" [ {m.roughnness} ]\n')
        output_lines.append(f'    "rgb reflectance" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
    if m.type == MATERIAL_DIFFUSE:
        output_lines.append('    "string type" [ "diffuse" ]\n')
        output_lines.append(f'    "rgb reflectance" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
    if m.type == MATERIAL_DIELECTRIC:
        output_lines.append('    "string type" [ "dielectric" ]\n')
        output_lines.append(f'    "float roughness" [ {m.eta} ]\n')
    if m.type == MATERIAL_DIFFUSE_TRANSMISSION:
        output_lines.append('    "string type" [ "diffusetransmission" ]\n')
        output_lines.append(f'    "rgb reflectance" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
        output_lines.append(f'    "rgb transmittance" [ {m.transmittance.x} {m.transmittance.y} {m.transmittance.z} ]\n')
    if m.type == MATERIAL_CONDUCTOR:
        output_lines.append('    "string type" [ "conductor" ]\n')
        output_lines.append(f'    "float roughness" [ {m.roughnness} ]\n')
        output_lines.append(f'    "rgb reflectance" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
    
    
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
        
        # if re.search(f'material-testball', line):
        #     output_lines.append(f'    \"string filename\" [ \"matBallsOutput/material-testball{index}.png\" ]\n')
        #     continue
        output_lines.append(line)
        
        if re.search(PATTERN_BEGIN, line):
            marker_found = True
    
    output_lines_2 = []
    for line in output_lines:
        if re.search(f'material-testball', line):
            output_lines_2.append(f'    \"string filename\" [ \"matBallsOutput/{m.name}.png\" ]\n')
            continue
        output_lines_2.append(line)

    
    with open(f'material-testball/{m.name}.pbrt', 'w') as file:
        for string in output_lines_2:
            file.write(string)
    

if __name__ == '__main__':
    for i in range(53):
        replace_Mat(i)