import os
import math

# frames = 1
# angle = (2 * math.pi) / frames
# current_angle = 0

# for i in range(frames):
# 	x = 110 + 600 * math.cos(current_angle)
# 	y = 10 + 600 * math.sin(current_angle)
# 	current_angle += angle
# 	myfile = open('test_water_map.xml', 'w')
# 	myfile.write('''<scene ambiant="0.1" AA="2" reflection_depth="0" refraction_depth="10" resolution="1500"/>
# 	<camera position="(170, 30, -0)" lookat="(110, 30, 10)" fov="40"/>
# 	<light center="(%f, 50, %f)" intensity="0.5" color="#FFFFFF"/>
# 	<sphere center="(110, 30, 10)" color="#0077be" radius="20" bump="/Users/abiri/Desktop/earth_bump.tex" specular="/Users/abiri/Desktop/earth_specular.tex" texture="/Users/abiri/Desktop/earth_diffuse.tex"/>''' % (x, y))
# 	myfile.close()
# 	print("GENERATING IMAGE %d" % (i,))
# 	os.system('./rtv1 --no_window earth%d.bmp test_water_map.xml' % (i,))
# exit (0)

for i in range(1, 120):
    myfile = open('test_water_map.xml', 'w')
    myfile.write('''<scene ambiant="0.1" AA="2" reflection_depth="0" refraction_depth="10" resolution="720"/>
<camera position="(10, 25, 10)" lookat="(10, 30, 110)" fov="40"/>
<light center="(0, 900, 300)" intensity="0.5" color="#FFFFFF"/>
<ssphere center="(0, 0, 0)" color="#FFFFFF" radius="1000"/>
<sphere center="(10, 30, 110)" color="#FFFFFF" radius="20"/>
<sphere center="(110, 70, 110)" color="#FFFFFF" radius="20"/>
<sphere center="(110, 70, 110)" color="#FFFFFF" radius="20"/>
<plane center="(0, 0, 0)" normal="(0, 1, 0)" color="#E3E3E3"/>
<plane center="(0, 50, 0)" color="#0077be" normal="(0, 1, 0)" bump="./animated_water/water%d.tex" transparency="0.8" refraction="2.6"/>''' % (i,))
    myfile.close()
    print("GENERATING IMAGE %d" % (i,))
    os.system('./rtv1 --no_window image%d.bmp test_water_map.xml' % (i,))
