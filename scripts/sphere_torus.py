import math
import sys

def create_sphere_torus():
    inner_radius = int(input("Enter the inner radius : "))
    outer_radius = int(input("Enter the outer radius : "))
    iterations = int(input("Enter the iterations   : "))
    torus_template = '''<sphere radius="%f" center="(%f, %f, %f)"/>'''
    radius = (outer_radius - inner_radius) / 2
    for i in range(iterations):
        angle = i * ((math.pi * 2) / iterations)
        posx = (inner_radius + outer_radius) / 2 * math.cos(angle)
        posz = (inner_radius + outer_radius) / 2 * math.sin(angle)
        posy = 0
        print(torus_template % (radius, posx, posy, posz))

def create_wave_plane():
    step = float(input("Enter the iterations : "))
    sphere_template = '''<sphere radius="%f" center="(%f, %f, %f)"/>'''
    i = 0
    while i < 20:
        j = 0
        while j < 20:
            x = i
            z = j
            y = (math.cos(x) ** 2) * math.sin(z)
            print(sphere_template % (step, x, y, z))
            j += step
        i += step

def main():
    choice = input("What shape you want to create: - torus, wave\n")
    if choice == "torus":
        create_sphere_torus()
    elif choice == "wave":
        create_wave_plane()

if __name__ == "__main__":
    main()
