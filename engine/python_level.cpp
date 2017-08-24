#include "python_level.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef level_methods[] =
	{
		{ "spawn_background", PythonLevel::py_spawn_background, METH_VARARGS, "Spawns the background of a level." },
		{ "trigger_level_loaded", PythonLevel::py_trigger_level_loaded, METH_VARARGS, "Triggers that the level is loaded." },
		{ "trigger_level_update", PythonLevel::py_trigger_level_update, METH_VARARGS, "Triggers that the level is updating." },
		{ "trigger_level_shutdown", PythonLevel::py_trigger_level_shutdown, METH_VARARGS, "Triggers that the level is shutting down." },
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC initlevel(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"stingray.Level",
			"Stingray Level Python Module",
			-1,
			level_methods,
			NULL, NULL, NULL, NULL
		};
		app_module = PyModule_Create(&moduledef);
		if (app_module == NULL)
			return NULL;
		PyModule_AddObject(stingray_module, "Level", app_module);
		return app_module;
	}

	PyObject* PythonLevel::py_spawn_background(PyObject* self, PyObject* args)
	{
		PyObject* level = Py_None;
		LevelPtr level_ptr;

		if (PyArg_ParseTuple(args, "O", &level))
		{
			level_ptr = (LevelPtr) PyLong_AsVoidPtr(level);
			if (level_ptr != nullptr) {
				PythonPlugin::get_api()._script->Level->spawn_background(level_ptr);
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonLevel::py_trigger_level_loaded(PyObject* self, PyObject* args)
	{
		PyObject* level = Py_None;
		LevelPtr level_ptr;

		if (PyArg_ParseTuple(args, "O", &level))
		{
			level_ptr = (LevelPtr) PyLong_AsVoidPtr(level);
			if (level_ptr != nullptr) {
				PythonPlugin::get_api()._script->Level->trigger_level_loaded(level_ptr);
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonLevel::py_trigger_level_update(PyObject* self, PyObject* args)
	{
		PyObject* level = Py_None;
		LevelPtr level_ptr;

		if (PyArg_ParseTuple(args, "O", &level))
		{
			level_ptr = (LevelPtr) PyLong_AsVoidPtr(level);
			if (level_ptr != nullptr) {
				PythonPlugin::get_api()._script->Level->trigger_level_update(level_ptr);
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonLevel::py_trigger_level_shutdown(PyObject* self, PyObject* args)
	{
		PyObject* level = Py_None;
		LevelPtr level_ptr;

		if (PyArg_ParseTuple(args, "O", &level))
		{
			level_ptr = (LevelPtr) PyLong_AsVoidPtr(level);
			if (level_ptr != nullptr) {
				PythonPlugin::get_api()._script->Level->trigger_level_shutdown(level_ptr);
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}
}
