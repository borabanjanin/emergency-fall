#!/usr/bin/python
from visual import *
import math
ORIGIN = (0,0,0)



scene = display(title='The Subject :)',x=0, y=0, width=400, height=400,center=(0,0,0))
scene.autoscale = False
dir(scene)
scene.select()
f = frame()	
body = ellipsoid(frame=f, pos=ORIGIN, length=2.0, height=5.0, width=2.0)
head = sphere(frame=f, pos=(0,3.5,0.0), radius=1.0)
arm1 = cylinder(frame=f, pos=(0.5,1.5,0.0), axis=(2.0,0,0), radius=0.3, color=color.red)
arm2 = cylinder(frame=f, pos=(-0.5,1.5,0.0), axis=(-2.0,0,0), radius=0.3, color=color.blue)
z = frame()
leg1 = cylinder(frame=z, pos=(0.5,-1.5,0.0), axis=(0.2,-4.0,0.0), radius=0.3, color=color.cyan)
leg2 = cylinder(frame=z, pos=(-0.5,-1.5,0.0), axis=(-0.2,-4.0,0.0), radius=0.3, color=color.green)

chest = {'x_angle': 0.0, 'y_angle': 0.0, 'z_angle': 0.0}
thigh = {'x_angle': 0.0, 'y_angle': 0.0, 'z_angle': 0.0}

chest_change = {'x_angle': 0.0, 'y_angle': 0.0, 'z_angle': 0.0}
thigh_change = {'x_angle': 0.0, 'y_angle': 0.0, 'z_angle': 0.0}

def update_thigh():
#	thigh_vector1 = vector(cos(radians(thigh['x_angle']))*4,cos(radians(thigh['z_angle']))*4,cos(radians(thigh['y_angle']))*4)
#	thigh_vector2 = vector(cos(radians(thigh['x_angle']))*4,cos(radians(thigh['z_angle']))*4,cos(radians(thigh['y_angle']))*4)
#	leg1.axis = thigh_vector1
#	leg2.axis = thigh_vector2
	z.rotate(angle=radians(thigh_change['z_angle']),axis=(1,0,0),origin=(0.0,-1.5,0.0))
	#z.rotate(angle=radians(thigh['y_angle']),axis=(0,1,0),origin=(0.0,-1.5,0.0))
	#z.rotate(angle=radians(thigh['z_angle']),axis=(0,0,1),origin=(0.0,-1.5,0.0))

def update_chest():
	f.rotate(angle=radians(chest['z_angle']),axis=(1,0,0),origin=(0.0,-1.5,0.0))


print "Module Started Up"
try:
	chest_run = 0
	thigh_run = 0
	while True:
		rate(20)
		packet = sys.stdin.readline()
		packet = sys.stdin.readline()
		packet = sys.stdin.readline()
		packet = sys.stdin.readline().strip('\n')
		if len(packet) != 0:	
			tokens = packet.split(" ")
			if tokens[0] == 'c':
				if chest_run == 0:
					chest_run = 1
					chest['x_angle'] = float(tokens[1])
					chest['y_angle'] = float(tokens[2])
					chest['z_angle'] = float(tokens[3])
				else:
					chest['x_angle'] = chest['x_angle'] - float(tokens[1])
					chest['y_angle'] = chest['y_angle'] - float(tokens[2])
					chest['z_angle'] = chest['y_angle'] - float(tokens[3])
					update_chest()
			elif tokens[0] == 't':
				if thigh_run == 0:
					thigh_run = 1
					thigh['x_angle'] = -(float(tokens[1]) + 90)
					thigh['y_angle'] = -(float(tokens[2]) + 90)
					thigh['z_angle'] = -(float(tokens[3]) + 90)
					update_thigh()
				else:
					print thigh_change['z_angle']
					thigh_change['x_angle'] =  -(float(tokens[1]) + 90) - thigh['x_angle']
					thigh_change['y_angle'] = -(float(tokens[2]) + 90) - thigh['y_angle']
					thigh_change['z_angle'] = -(float(tokens[3]) + 90) - thigh['z_angle']
					thigh['x_angle'] = -(float(tokens[1]) + 90)
					thigh['y_angle'] = -(float(tokens[2]) + 90)
					thigh['z_angle'] = -(float(tokens[3]) + 90)
					update_thigh()
					print thigh_change['z_angle']
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
