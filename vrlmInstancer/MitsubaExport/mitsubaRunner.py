import mitsuba as mi
import os, xml

mi.set_variant('scalar_rgb')
scene_file_name = 'sceneFile.xml'
# scene_file_name = 'scn.xml'
# scene_file_name = 'mainScene.xml'

scene_file_name_stripped, _ = os.path.splitext(scene_file_name)

# scene = mi.load_file('mainScene.xml')
scene = mi.load_file(scene_file_name)
print('loaded')

img = mi.render(scene)

mi.Bitmap(img).write('Output/' + scene_file_name_stripped + '.exr')