#Python Boot Script for the Stingray Engine
import stingray

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

print "Python Boot File"
world = None
viewport = None
shading_env = None

def on_setup():
	global world
	global viewport
	global shading_env
	world = Application.new_world(DISABLE_SOUND = 1)
	viewport = Application.create_viewport(world, "default")
	#shading_env = World.create_shading_environment(world, "core/stingray_renderer/environments/midday/midday")
	#camera_unit = World.spawn_unit(world, "core/units/camera")

def on_update( delta_time ):
	pass

def on_shutdown():
	global world
	Application.release_world(world)

stingray.register_setup(on_setup)
stingray.register_update(on_update)
stingray.register_shutdown(on_shutdown)