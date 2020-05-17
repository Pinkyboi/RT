import os
import cv2
import numpy as np
import math
from os.path import isfile, join

exec_path = "/Users/merras/Desktop/RT/rt"
save_path = "/Users/merras/Desktop/RT/scripts/"
keep_frames = False
frame_count = 60
frames_per_second = 30
resolution = 1000
rendered_files = []

###################################################
#            SCENE GENERATION FUNCTION            #
###################################################

def generate_rotary_camera(index, center=(0, 0, 0), radius=5.0, rotation=0.5, rotation_offset=0, y=0):
    scene = '''<scene ambiant="0.5" AA="4" resolution="%d" light_samples="1" refraction_depth="0" reflection_depth="0"/>
    <camera position="(%f, %f, %f)" lookat="(%f, %f, %f)" fov="40"/>
    <light center="(0, 5, 20)"  radius="1" intensity="0.5" color="#FFFFFF"/>
    <sphere center="(0, 0, 0)" mapping_position="(0, 0, 3)" color="#0000FF" radius="5" texture_mapping="../rt-scenes/textures/FABRIC_DIFF.tex" bump_mapping="../rt-scenes/textures/FABRIC_NORM.tex"/>\n'''
    angle = ((index / frame_count) * (rotation * 2.0 * math.pi)) - rotation_offset * 2.0 * math.pi
    newx = center[0] + radius * math.cos(angle)
    newz = center[2] + radius * math.sin(angle)
    return (scene % (resolution, newx, y, newz, center[0], center[1], center[2]))

###################################################
#               BUILTIN FUNCTIONS                 #
###################################################

def render_current_scene(scene, index):
    with open(join(save_path, "autorender_scene.xml"), 'w+') as scene_file:
        scene_file.write(scene)
        scene_file.close()
        image_filename = join(save_path, "render_" + str(index) + ".bmp")
        print ("rendering frame : %d" % (index,))
        exit_status = os.system(exec_path + " --no_window " + image_filename + " " + join(save_path, "autorender_scene.xml"))
        if exit_status != 0:
            print("Rendering frame %d failed with exit code : %d" % (index, exit_status))
            exit(1)
        rendered_files.append(image_filename)
        return
    print("Cannot save temp scene file to " + save_path)
    exit(1)

def cleanup_saved_files():
    for filename in rendered_files:
        os.remove(filename)

def convert_frames_to_video():
    frame_array = []
    for i in range(len(rendered_files)):
        filename = rendered_files[i]
        img = cv2.imread(filename)
        height, width, layers = img.shape
        size = (width,height)
        frame_array.append(img)
    out = cv2.VideoWriter(join(save_path, "render_video.mp4"), cv2.VideoWriter_fourcc(*'mp4v'), frames_per_second, size)
    for i in range(len(frame_array)):
        out.write(frame_array[i])
    out.release()

def main():
    if exec_path == None:
        print ("Please enter exec path")
        exit(1)
    if save_path == None:
        print ("Please enter save path")
        exit(1)
    for index in range(frame_count):
        # SCENE GENERATION FUNCTION v
        scene = generate_rotary_camera(index, center=(0, 0, 0), radius=20, rotation=0.5, rotation_offset=0.2)
        # SCENE GENERATION FUNCTION ^
        render_current_scene(scene, index)
    convert_frames_to_video()
    if not keep_frames:
        cleanup_saved_files()

if __name__ == "__main__":
    main()
