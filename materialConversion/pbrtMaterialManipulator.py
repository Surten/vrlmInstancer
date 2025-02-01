import re
from dataclasses import dataclass

FILE_NAME = "material-testball/scene-v4.pbrt"
FILE_NAME_MATERIALS = "materials.mat"
PATTERN_BEGIN = "MakeNamedMaterial \"Material\""
PATTERN_END = "MakeNamedMaterial"
#BF_blue01 0.0902 0.07843 0.5804 MAT-coateddiffuse roughness [ 0.6 ] reflectance [ 0.0902 0.07843 0.5804 ]

@dataclass
class Vector3():
    x: float
    y: float
    z: float

@dataclass
class Material():
    name: str
    type: str
    roughnness: float
    reflectance: Vector3
    transmittance: Vector3


def read_material(material_text):
    words = material_text.split(" ")
    m = Material()
    if words[4]
    m.name = words[0]
    

def add_new_material(index):
    output_lines = []
    try:
        with open(FILE_NAME_MATERIALS, 'r') as file_in:
            lines = file_in.readlines()
    except FileNotFoundError:
        print("File not found")
    
    
    
    return output_lines

def replace_Mat():
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
                output_lines.append(add_new_material())
                output_lines.append(line)
            
            continue
            
        output_lines.append(line)
        
        if re.search(PATTERN_BEGIN, line):
            marker_found = True
            

            