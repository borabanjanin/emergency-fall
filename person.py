#!/usr/bin/python
from visual import *
import math
ORIGIN = (0,0,0)



scene = display(title='The Subject :)',x=0, y=0, width=400, height=400,center=(0,0,0))
scene.autoscale = False
dir(scene)
scene.select()
	
body = ellipsoid(pos=ORIGIN, length=2.0, height=5.0, width=2.0)
head = sphere(pos=(0,3.5,0.0), radius=1.0)
arm1 = cylinder(pos=(0.5,1.5,0.0), axis=(2.0,0,0), radius=0.3, color=color.red)
arm2 = cylinder(pos=(-0.5,1.5,0.0), axis=(-2.0,0,0), radius=0.3, color=color.blue)
leg1 = cylinder(pos=(0.5,-1.5,0.0), axis=(0.2,-4.0,0.0), radius=0.3, color=color.cyan)
leg2 = cylinder(pos=(-0.5,-1.5,0.0), axis=(-0.2,-4.0,0.0), radius=0.3, color=color.green)

chest = {'x_angle': 0.0, 'y_angle': 0.0, 'z_angle': 0.0}
thigh = {'x_angle': 0.0, 'y_angle': 0.0, 'z_angle': 0.0}

def update_thigh():
	thigh_vector1 = vector(0.2,cos(radians(thigh['y_angle']))*4,cos(radians(thigh['z_angle']))*4)
	thigh_vector2 = vector(-0.2,cos(radians(thigh['y_angle']))*4,cos(radians(thigh['z_angle']))*4)
	print thigh_vector1
	print thigh_vector1
	leg1.axis = thigh_vector1
	leg2.axis = thigh_vector2


print "Module Started Up"
try:
	while True:
		packet = sys.stdin.readline().strip('\n')
		if len(packet) != 0:	
			tokens = packet.split(" ")
			if tokens[0] == 'c':
				chest['x_angle'] = float(tokens[1])
				chest['y_angle'] = float(tokens[2])
				chest['z_angle'] = float(tokens[3])
			elif tokens[0] == 't':
				thigh['x_angle'] = float(tokens[1])
				thigh['y_angle'] = float(tokens[2])
				thigh['z_angle'] = float(tokens[3])
				update_thigh()
except KeyboardInterrupt:
	print "Bye"
	

#	sue = sphere(pos=vector(-1,0,0) , radius=0.5, color=color.green)
#	bob = sphere(pos=vector(1,1,0) ,radius=0.5, color=color.red)
#	change = vector(bob.pos-sue.pos)
#	john = arrow(pos=sue.pos, axis=change, color=color.yellow)
#	for i in range(100):
#		rate(10)
#		change = vector(bob.pos-sue.pos)
#		change = change * i/100
#		john.axis = change
