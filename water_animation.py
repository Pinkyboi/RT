import os
import math

frames = 30
step =1.1
angle = (2 * math.pi) / frames
current_angle = 0
radius = 5
x = 3#radius * math.cos(current_angle)
y = 3#radius * math.sin(current_angle)

for i in range(frames):
        current_angle += angle
        myfile = open('test_water_map.xml', 'w')
        myfile.write('''<scene ambiant="0.5" AA="0" reflection_depth="0" refraction_depth="0" resolution="50"/>
<camera position="(%f, 1, %f)" lookat="(0.5, 1, 0.5)" fov="40"/>
<light center="(5, 100, 110)" intensity="0.5" color="#FFFFFF"/>
<ssphere center="(10, 30, 110)" color="#FF00FF" radius="20" refraction="2.01" transparency="0.95"/>
<ssphere center="(10, 30, 110)" color="#FF00FF" radius="3" refraction="2.6" transparency="0"/>
<ssphere center="(15, 30, 110)" color="#FF00FF" radius="3" refraction="2.6" transparency="0.5"/>
<ssphere center="(10, 30, 115)" color="#FF00FF" radius="3" refraction="2.6" transparency="0.8"/>
<fractal center="(0.5, 1, 0.5)" color="#FF00FF"/>
<ssphere center="(2.5, 1, -1)" color="#FFFFFF" radius="1" reflection="0.6" transparency="0.5" refraction="2.6"/>
<plane center="(0, 0, 0)" normal="(0.1, 1, 0.1)" color="#FFFFFF" reflection="0.2"/>''' % (x, y))
        myfile.close()
        print("GENERATING IMAGE %d" % (i,))
        os.system('./rtv1 --no_window ~/goinfre/fractal%d.bmp test_water_map.xml' % (i,))
        x /= step
        y /= step
exit (0)

# for i in range(1, 120):
#     myfile = open('test_water_map.xml', 'w')
#     myfile.write('''<scene ambiant="0.1" AA="2" reflection_depth="0" refraction_depth="10" resolution="720"/>
# <camera position="(10, 25, 10)" lookat="(10, 30, 110)" fov="40"/>
# <light center="(0, 900, 300)" intensity="0.5" color="#FFFFFF"/>
# <ssphere center="(0, 0, 0)" color="#FFFFFF" radius="1000"/>
# <sphere center="(10, 30, 110)" color="#FFFFFF" radius="20"/>
# <sphere center="(110, 70, 110)" color="#FFFFFF" radius="20"/>
# <sphere center="(110, 70, 110)" color="#FFFFFF" radius="20"/>
# <plane center="(0, 0, 0)" normal="(0, 1, 0)" color="#E3E3E3"/>
# <plane center="(0, 50, 0)" color="#0077be" normal="(0, 1, 0)" bump="./animated_water/water%d.tex" transparency="0.8" refraction="2.6"/>''' % (i,))
#     myfile.close()
#     print("GENERATING IMAGE %d" % (i,))
#     os.system('./rtv1 --no_window image%d.bmp test_water_map.xml' % (i,))
