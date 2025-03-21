import mitsuba as mi
import mitsuba as mi

mi.set_variant('scalar_rgb')

scene = mi.load_file('mainScene.xml')
# scene = mi.load_file('material-testball/scene_v0.6.xml')
# scene = mi.load_file('volumetric-caustic/scene_v3.xml')

img = mi.render(scene)

mi.Bitmap(img).write('room215.exr')