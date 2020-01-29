import os

for i in range(1, 120):
    myfile = open('test_water_map.xml', 'w')
    myfile.write('''<scene ambiant="0.5" AA="2" reflection_depth="0" refraction_depth="10" resolution="720"/>
<camera position="(10, 50, 110)" lookat="(110, 30, 10)" fov="40"/>
<light center="(0, 1000, 300)" intensity="0.5" color="#FFFFFF"/>
<ssphere center="(10, 30, 110)" color="#FF00FF" radius="20"/>
<plane center="(0, 0, 0)" normal="(0, 1, 0)" color="#E3E3E3"/>
<sphere center="(110, 30, 10)" color="#0077be" radius="20" refraction="2.6" transparency="0.3" bump="./animated_water/water%d.tex"/>''' % (i,))
    myfile.close()
    print("GENERATING IMAGE %d" % (i,))
    os.system('./rtv1 --no_window test_water_map.xml')
