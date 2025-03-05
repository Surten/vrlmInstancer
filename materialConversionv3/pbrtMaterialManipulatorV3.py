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
    sigma: float = 0
    roughnness: float = 0
    uroughnness: float = 0
    vroughnness: float = 0
    diffuse: Vector3 = Vector3()
    specular: Vector3 = Vector3()
    transmittance: Vector3 = Vector3()
    reflectance: Vector3 = Vector3()


def read_material(material_text):
    words = material_text.split(" ")
    m = Material()
    m.name = words[0]
    m.type = words[4]
    index = 5
    while index < len(words):
        if words[index] == '\n':
            index += 1
            continue
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
        elif words[index] == 'Kr':
            index += 2
            m.reflectance = Vector3(float(words[index]), float(words[index+1]), float(words[index+2]))
            index += 4
        elif words[index] == 'Kt':
            index += 2
            m.transmittance = Vector3(float(words[index]), float(words[index+1]), float(words[index+2]))
            index += 4
        elif words[index] == 's':
            index += 2
            m.sigma = float(words[index])
            index += 2
        elif words[index] == 'ur':
            index += 2
            m.uroughnness = float(words[index])
            index += 2
        elif words[index] == 'vr':
            index += 2
            m.vroughnness = float(words[index])
            index += 2
        elif words[index] == 'in':
            index += 2
            m.eta = float(words[index])
            index += 2
        elif words[index] == 'fl':
            index += 2
            m.reflectance = Vector3(float(words[index]), float(words[index+1]), float(words[index+2]))
            index += 4
        elif words[index] == 'tt':
            index += 2
            m.transmittance = Vector3(float(words[index]), float(words[index+1]), float(words[index+2]))
            index += 4
        else:
            print("unrecognized")

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
    if m.type == MATERIAL_MATTE:
        output_lines.append('    "string type" [ "matte" ]\n')
        output_lines.append(f'    "rgb Kd" [ {m.diffuse.x} {m.diffuse.y} {m.diffuse.z} ]\n')
        output_lines.append(f'    "float sigma" [ {m.sigma} ]\n')
    if m.type == MATERIAL_GLASS:
        output_lines.append('    "string type" [ "glass" ]\n')
        output_lines.append(f'    "rgb Kr" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
        output_lines.append(f'    "rgb Kt" [ {m.transmittance.x} {m.transmittance.y} {m.transmittance.z} ]\n')
        output_lines.append(f'    "float in" [ {m.sigma} ]\n')
    if m.type == MATERIAL_SUBSTRATE:
        output_lines.append('    "string type" [ "substrate" ]\n')
        output_lines.append(f'    "rgb Kd" [ {m.diffuse.x} {m.diffuse.y} {m.diffuse.z} ]\n')
        output_lines.append(f'    "rgb Ks" [ {m.specular.x} {m.specular.y} {m.specular.z} ]\n')
        output_lines.append(f'    "float uroughness" [ {m.uroughnness} ]\n')
        output_lines.append(f'    "float vroughness" [ {m.vroughnness} ]\n')
    if m.type == MATERIAL_TRANSLUCENT:
        output_lines.append('    "string type" [ "translucent" ]\n')
        output_lines.append(f'    "rgb Kd" [ {m.diffuse.x} {m.diffuse.y} {m.diffuse.z} ]\n')
        output_lines.append(f'    "rgb Ks" [ {m.specular.x} {m.specular.y} {m.specular.z} ]\n')
        output_lines.append(f'    "rgb fl" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
        output_lines.append(f'    "float roughness" [ {m.roughnness} ]\n')
        output_lines.append(f'    "rgb tt" [ {m.transmittance.x} {m.transmittance.y} {m.transmittance.z} ]\n')
    if m.type == MATERIAL_UBER or m.type == MATERIAL_SHINY:
        output_lines.append('    "string type" [ "uber" ]\n')
        output_lines.append(f'    "rgb Kd" [ {m.diffuse.x} {m.diffuse.y} {m.diffuse.z} ]\n')
        output_lines.append(f'    "rgb Ks" [ {m.specular.x} {m.specular.y} {m.specular.z} ]\n')
        output_lines.append(f'    "rgb Kr" [ {m.reflectance.x} {m.reflectance.y} {m.reflectance.z} ]\n')
        output_lines.append(f'    "float roughness" [ {m.roughnness} ]\n')

    
    
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
        
        # if re.search(f'material-testball', line):
        #     output_lines.append(f'    \"string filename\" [ \"matBallsOutput/{m.name}_{index}.png\" ]\n')
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