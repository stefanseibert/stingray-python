#Python Boot Script for the Stingray Engine
import stingray
import threading
import sys
import os

#Cached Modules
Application = stingray.Application
CaptureFrame = stingray.CaptureFrame
Keyboard = stingray.Keyboard
Level = stingray.Level
ShadingEnvironment = stingray.ShadingEnvironment
StoryTeller = stingray.StoryTeller
Unit = stingray.Unit
Window = stingray.Window
World = stingray.World

print("Python Boot File")
world = None
viewport = None
shading_env = None
counter = 0
PATH_SET = False
IMG_NAME = "RECOGNIZE.png"

def on_setup():
	print(dir(stingray.World))

def on_update( delta_time ):
	# The project path is set from the engine and needs to be added as path
	if hasattr(stingray, 'PROJECT_PATH') and not PATH_SET:
		sys.path.append(stingray.PROJECT_PATH)
		filename = stingray.PROJECT_PATH + "\\" + IMG_NAME
		if os.path.isfile(filename):
			recognize(filename)

	global counter
	if counter < 20:
		counter = counter + 1
		world = Application.main_world()
		World.spawn_unit(world, "core/units/primitives/cube_primitive", counter, counter, counter)

def on_shutdown():
	global world
	Application.release_world(world)


def recognize(filename):
	try:
		import classify_image
		thread = threading.Thread(target=classify_image.main, args=(filename))
		thread.start()
		return_val = thread.join()
	except SystemExit as e:
		pass

stingray.register_setup(on_setup)
stingray.register_update(on_update)
stingray.register_shutdown(on_shutdown)
