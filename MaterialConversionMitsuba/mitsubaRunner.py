import mitsuba as mi
import os


FOLDER = 'material-testball'
OUTPUT_FOLDER = 'Output'
# scene_file_name = 'material-testball/scene_v0.6.xml'
mi.set_variant('scalar_rgb')


if False:
    FOLDER = 'staticSceneCam'
    OUTPUT_FOLDER = 'Output'
    # file = 'blackboard_header.xml'
    file = 'cam01.xml'
    scene_file_name_stripped, _ = os.path.splitext(file)
    file_path = os.path.join(FOLDER, file)
    scene = mi.load_file(file_path)

        
    print('Loaded ' + scene_file_name_stripped)
    img = mi.render(scene)
    bitmap_image = mi.Bitmap(img)

    bitmap_image = bitmap_image.convert(mi.Bitmap.PixelFormat.RGB, mi.Struct.Type.UInt8, True)
    print('Rendered ' + scene_file_name_stripped)
    bitmap_image.write(OUTPUT_FOLDER + '/' + scene_file_name_stripped + '.png')
    print('Saved ' + scene_file_name_stripped)

    exit()


else:







    if not os.path.exists(OUTPUT_FOLDER):
        os.mkdir(OUTPUT_FOLDER)
    files = os.listdir('material-testball')
    files = [f for f in files if (f[0] == 'B' or f[0] == 'C') and (f[1] == 'F' or f[1] == 'H')]

    start_from = len(os.listdir(OUTPUT_FOLDER))
    # start_from = 0
    index = 0
    for file in files:
        
        scene_file_name_stripped, _ = os.path.splitext(file)
        if index < start_from:
            print('Skipped ' + scene_file_name_stripped)
            index += 1
            continue
        file_path = os.path.join(FOLDER, file)
        try:
            scene = mi.load_file(file_path)
        except Exception as e:
            print(e)
            
        print('Loaded ' + scene_file_name_stripped)
        img = mi.render(scene)
        mi.Bitmap(img).write('out.exr')
        bitmap_image = mi.Bitmap(img)

        bitmap_image = bitmap_image.convert(mi.Bitmap.PixelFormat.RGB, mi.Struct.Type.UInt8, True)
        print('Rendered ' + scene_file_name_stripped)
        bitmap_image.write(OUTPUT_FOLDER + '/' + scene_file_name_stripped + '.png')
        print('Saved ' + scene_file_name_stripped)