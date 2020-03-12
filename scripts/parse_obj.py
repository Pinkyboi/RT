import sys
import math

if len(sys.argv) != 2:
    print("Wrong number of arguments")
    exit(1)
objfile = open(sys.argv[1])
lines = objfile.read().split("\n")
vertices = []
faces = []
spheres = []
for line in lines:
    coords = line.split()
    if len(coords) >= 4:
        if coords[0] == 'v':
            vertices.append((float(coords[1]), float(coords[2]), float(coords[3])))
        elif coords[0] == 'f':
            faces.append((vertices[int(coords[1].split('/')[0]) - 1], vertices[int(coords[2].split('/')[0]) - 1], vertices[int(coords[3].split('/')[0]) - 1]))
maxx = -math.inf
maxy = -math.inf
maxz = -math.inf
minx = math.inf
miny = math.inf
minz = math.inf
for face in faces:
    maxx = max((maxx, face[0][0], face[1][0], face[2][0]))
    maxy = max((maxy, face[0][1], face[1][1], face[2][1]))
    maxz = max((maxz, face[0][2], face[1][2], face[2][2]))
    minx = min((minx, face[0][0], face[1][0], face[2][0]))
    miny = min((miny, face[0][1], face[1][1], face[2][1]))
    minz = min((minz, face[0][2], face[1][2], face[2][2]))
    print ('<triangle center="(%f, %f, %f)" side1="(%f, %f, %f)" side2="(%f, %f, %f)" normal="(0, 1, 0)" color="#FF00FF"/>' % (face[0][0] , face[0][1], face[0][2], face[1][0] , face[1][1], face[1][2], face[2][0] , face[2][1], face[2][2]))
print("interaval : [%f %f][%f %f][%f %f]" % (minx, maxx,miny, maxy, minz, maxz), file=sys.stderr)
print("center    : %f %f %f" % ((maxx + minx) / 2, (maxy + miny) / 2, (maxz + minz) / 2), file=sys.stderr)
