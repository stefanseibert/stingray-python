#include "python_application.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef application_methods[] =
	{
		{ "argv", PythonApplication::py_argv, METH_VARARGS, "Returns the engine command-line arguments." },
		{ "new_world", (PyCFunction) PythonApplication::py_new_world, METH_VARARGS | METH_KEYWORDS, "Creates a new World." },
		{ "release_world", PythonApplication::py_release_world, METH_VARARGS, "Releases a World." },
		{ "create_viewport", PythonApplication::py_create_viewport, METH_VARARGS, "Creates a Viewport." },
		{ "destroy_viewport", PythonApplication::py_destroy_viewport, METH_VARARGS, "Destroys a Viewport." },
		{ "main_world", PythonApplication::py_main_world, METH_VARARGS, "Returns the main world." },
		{ "can_get", PythonApplication::py_can_get, METH_VARARGS, "Indicates if a resource is available" },
		{ "render_world", PythonApplication::py_render_world, METH_VARARGS, "Renders the specified world, with the specified shading environment, using the specified camera, in the specified viewport." },
		/*{ "autoload_resource_package", PythonApplication::py_autoload_resource_package, METH_VARARGS, "Autoloads the given package if available." },*/
		{ "quit", PythonApplication::py_quit, METH_VARARGS, "Quits the Application." },
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC PyInit_Application(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"Application",
			"Stingray Application Python Module",
			-1,
			application_methods,
			NULL, NULL, NULL, NULL
		};

		app_module = PyModule_Create(&moduledef);
		if (app_module == NULL)
			return NULL;
		PyModule_AddObject(stingray_module, "Application", app_module);
		return app_module;
		

	}

	PyObject* PythonApplication::py_argv(PyObject* self, PyObject* args)
	{
		Array<char> buf(256, PythonPlugin::get_allocator());
		MultipleStringsBuffer arguments;
		arguments.num_strings = 0;
		arguments.s = (char**)&buf[0];
		unsigned required_size = PythonPlugin::get_api()._script->Application->argv(&arguments, 256);
		if (required_size > 256)
		{
			buf.resize(required_size);
			arguments.num_strings = 0;
			arguments.s = (char**)&buf[0];
			PythonPlugin::get_api()._script->Application->argv(&arguments, required_size);
		}

		PyObject *argument_list = PyList_New(0);
		for (unsigned i = 0; i < arguments.num_strings; ++i)
		{
			PyObject* py_argument = Py_BuildValue("s", arguments.s[i]);
			Py_IncRef(py_argument);
			PyList_Append(argument_list, py_argument);
			PythonPlugin::check_exceptions();
		}
		Py_IncRef(argument_list);
		return argument_list;
	}

	PyObject* PythonApplication::py_new_world(PyObject* self, PyObject* args, PyObject* keywords)
	{
		const char *keyword_list[] = { "DISABLE_APEX_CLOTH", "DISABLE_PHYSICS", "DISABLE_RENDERING",
										"DISABLE_SOUND", "ENABLE_REPLAY", "APEX_LOD_RESOURCE_BUDGET" }; // New World Flags
		PyObject *empty = PyTuple_New(0); // pure keyword functions unsupported, but we do not need args
		int disable_apex_cloth = INT_MAX;
		int disable_physics = INT_MAX;
		int disable_rendering = INT_MAX;
		int disable_sound = INT_MAX;
		int enable_replay = INT_MAX;
		float apex_lod_budget = 0.0f;
		if (PyArg_ParseTupleAndKeywords(empty, keywords, "|iiiiif", const_cast<char**>(keyword_list),&disable_apex_cloth, 
										&disable_physics, &disable_rendering, &disable_sound,&enable_replay, &apex_lod_budget))
		{
			// TODO PUSH get_default_world_settings to stingray plugin api
			CApiWorldConfig c = PythonPlugin::get_api()._script->Application->get_default_world_settings();
			if (disable_apex_cloth != INT_MAX)
				c.physics_world_settings.apex_cloth = !disable_apex_cloth;
			if (disable_physics != INT_MAX)
				c.disable_physics = disable_physics;
			if (disable_rendering != INT_MAX)
				c.disable_rendering = disable_rendering;
			if (disable_sound != INT_MAX)
				c.disable_sound = disable_sound;
			if (enable_replay != INT_MAX)
				c.enable_replay = enable_replay;
			if (disable_apex_cloth != INT_MAX && apex_lod_budget != 0.0f)
				c.physics_world_settings.apex_lod_resource_budget = apex_lod_budget;
			WorldPtr world_ptr = PythonPlugin::get_api()._script->Application->new_world(&c);
			PyObject* python_world_ptr = PyLong_FromVoidPtr(world_ptr);
			Py_IncRef(python_world_ptr);
			return python_world_ptr;
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonApplication::py_release_world(PyObject* self, PyObject* args)
	{
		PyObject *python_handle;
		PyArg_ParseTuple(args, "O", &python_handle);
		WorldPtr world_ptr = (WorldPtr) PyLong_AsVoidPtr(python_handle);
		if (world_ptr) {
			PythonPlugin::get_api()._script->Application->release_world(world_ptr);
			Py_DecRef(python_handle);
		}
		Py_RETURN_NONE;
	}

	PyObject* PythonApplication::py_create_viewport(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr;
		const char* viewport_name = "";
		IdString32 viewport_id;
		unsigned length;

		if (PyArg_ParseTuple(args, "Os#", &world, &viewport_name, &length))
		{
			viewport_id = IdString32(length, viewport_name);
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			if (world_ptr != nullptr && viewport_id.nonempty()) {
				ViewportPtr viewport_ptr = PythonPlugin::get_api()._script->Application->create_viewport(world_ptr, viewport_id.id());
				PyObject* python_viewport_ptr = PyLong_FromVoidPtr(viewport_ptr);
				Py_IncRef(python_viewport_ptr);
				return python_viewport_ptr;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonApplication::py_destroy_viewport(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr;
		PyObject* viewport = Py_None;
		ViewportPtr viewport_ptr;

		if (PyArg_ParseTuple(args, "OO", &world, &viewport))
		{
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			viewport_ptr = (ViewportPtr) PyLong_AsVoidPtr(viewport);
			if (world_ptr != nullptr && viewport_ptr != nullptr) {
				PythonPlugin::get_api()._script->Application->destroy_viewport(world_ptr, viewport_ptr);
				Py_RETURN_NONE;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonApplication::py_main_world(PyObject* self, PyObject* args)
	{
		WorldPtr world_ptr = PythonPlugin::get_api()._script->Application->main_world();
		if (world_ptr != nullptr) {
			PyObject* python_world_ptr = PyLong_FromVoidPtr(world_ptr);
			Py_IncRef(python_world_ptr);
			return python_world_ptr;
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonApplication::py_can_get(PyObject* self, PyObject* args)
	{
		const char* type_name = "";
		unsigned type_length;
		IdString64 type_id;
		const char* rsc_name = "";
		unsigned rsc_length;
		IdString64 rsc_id;

		if (PyArg_ParseTuple(args, "s#s#", &type_name, &type_length, &rsc_name, &rsc_length))
		{
			type_id = IdString64(type_length, type_name);
			rsc_id = IdString64(rsc_length, rsc_name);
			if (PythonPlugin::get_api()._script->Application->can_get(type_id.id(), rsc_id.id()))
				return Py_True;
		}

		PythonPlugin::check_exceptions();
		return Py_False;
	}

	PyObject* PythonApplication::py_render_world(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr;
		PyObject* camera = Py_None;
		CameraPtr camera_ptr;
		PyObject* viewport = Py_None;
		ViewportPtr viewport_ptr;
		PyObject* shading_env = Py_None;
		ShadingEnvironmentPtr shading_env_ptr;
		PyObject* window = Py_None;
		WindowPtr window_ptr = nullptr;

		if (PyArg_ParseTuple(args, "OOOO|O", &world, &camera, &viewport, &shading_env, &window))
		{
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			camera_ptr = (CameraPtr) PyLong_AsVoidPtr(camera);
			viewport_ptr = (ViewportPtr) PyLong_AsVoidPtr(viewport);
			shading_env_ptr = (ShadingEnvironmentPtr) PyLong_AsVoidPtr(shading_env);

			if (world_ptr != nullptr && camera_ptr != nullptr && viewport_ptr != nullptr && shading_env_ptr != nullptr) {
				if (window != Py_None)
					window_ptr = (WindowPtr) PyLong_AsVoidPtr(window);
				PythonPlugin::get_api()._script->Application->render_world(world_ptr, camera_ptr, viewport_ptr, shading_env_ptr, window_ptr);
				Py_RETURN_NONE;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonApplication::py_quit(PyObject* self, PyObject* args)
	{
		int exit_code;

		if (PyArg_ParseTuple(args, "i", &exit_code))
		{
			PythonPlugin::info("Python signals application exit.");
			PythonPlugin::get_api()._script->Application->quit(exit_code);
			return Py_True;
		}

		PythonPlugin::check_exceptions();
		PythonPlugin::info("Python tried to exit application but misses error code.");
		return Py_False;
	}

	PyObject* PythonApplication::py_autoload_resource_package(PyObject* self, PyObject* args)
	{
		// TODO: NO C_API FUNCTIONALITY - CALLING LUA

		const char* package_name = "";

		if (PyArg_ParseTuple(args, "s", &package_name))
		{
			lua_State *L = PythonPlugin::get_api()._lua->getscriptenvironmentstate();
			int err = PythonPlugin::get_api()._lua->lib_loadstring(L, "stingray.Application.autoload_resource_package()");
			if (err) {
				Py_RETURN_NONE;
			}
			PythonPlugin::get_api()._lua->pushstring(L, package_name);
			PythonPlugin::get_api()._lua->call(L, 1, 0);
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}
}
