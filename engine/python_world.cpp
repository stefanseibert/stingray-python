#include "python_world.h"

namespace PLUGIN_NAMESPACE
{
	using namespace stingray_plugin_foundation;

	static PyMethodDef world_methods[] =
	{
		{ "create_shading_environment", PythonWorld::py_create_shading_environment, METH_VARARGS, "Creates a Shading Environment." },
		{ "create_default_shading_environment", PythonWorld::py_create_default_shading_environment, METH_VARARGS, "Creates the default Shading Environment." },
		{ "destroy_shading_environment", PythonWorld::py_destroy_shading_environment, METH_VARARGS, "Destroys a Shading Environment." },
		{ "spawn_unit", PythonWorld::py_spawn_unit, METH_VARARGS, "Spawns a unit in the world." },
		{ "destroy_unit", PythonWorld::py_destroy_unit, METH_VARARGS, "Destroys a unit in the world." },
		{ "load_level", PythonWorld::py_load_level, METH_VARARGS, "Loads a level in the world." },
		{ "destroy_level", PythonWorld::py_destroy_level, METH_VARARGS, "Destroys a level in the world." },
		{ "update", PythonWorld::py_world_update, METH_VARARGS, "Updates the world." },
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC PyInit_World(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"World",
			"Stingray World Python Module",
			-1,
			world_methods,
			NULL, NULL, NULL, NULL
		};
		app_module = PyModule_Create(&moduledef);
		PyModule_AddObject(stingray_module, "World", app_module);
		return app_module;
	}

	PyObject* PythonWorld::py_create_shading_environment(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr = nullptr;
		const char* shading_env_name = "";
		IdString64 shading_env_id;
		unsigned length;

		if (PyArg_ParseTuple(args, "Os#", &world, &shading_env_name, &length))
		{
			shading_env_id = IdString64(length, shading_env_name);
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			if (world_ptr != nullptr && shading_env_id.nonempty()) {
				ShadingEnvironmentPtr env_ptr = PythonPlugin::get_api()._script->World->create_shading_environment(world_ptr, shading_env_id.id(), shading_env_name);
				PyObject* python_env_ptr = PyLong_FromVoidPtr(env_ptr);
				Py_IncRef(python_env_ptr);
				return python_env_ptr;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonWorld::py_destroy_shading_environment(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr = nullptr;
		PyObject* shading_env = Py_None;
		ShadingEnvironmentPtr shading_env_ptr = nullptr;

		if (PyArg_ParseTuple(args, "OO", &world, &shading_env))
		{
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			shading_env_ptr = (ShadingEnvironmentPtr) PyLong_AsVoidPtr(shading_env);
			if (world_ptr != nullptr && shading_env_ptr != nullptr) {
				PythonPlugin::get_api()._script->World->destroy_shading_environment(world_ptr, shading_env_ptr);
				Py_RETURN_NONE;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonWorld::py_create_default_shading_environment(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr = nullptr;

		if (PyArg_ParseTuple(args, "O", &world))
		{
			world_ptr = (WorldPtr)PyLong_AsVoidPtr(world);
			if (world_ptr != nullptr) {
				ShadingEnvironmentPtr env_ptr = PythonPlugin::get_api()._script->World->create_default_shading_environment(world_ptr);
				PyObject* python_env_ptr = PyLong_FromVoidPtr(env_ptr);
				Py_IncRef(python_env_ptr);
				return python_env_ptr;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonWorld::py_spawn_unit(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr = nullptr;
		const char* unit_name = "";
		IdString64 unit_id;
		unsigned length;

		float x_pos = 0.0f;
		float y_pos = 0.0f;
		float z_pos = 0.0f;
		float x_rot = 0.0f;
		float y_rot = 0.0f;
		float z_rot = 0.0f;
		float w_rot = 1.0f;

		if (PyArg_ParseTuple(args, "Os#|fffffff", &world, &unit_name, &length, &x_pos, &y_pos, &z_pos, &x_rot, &y_rot, &z_rot, &w_rot))
		{
			unit_id = IdString64(length, unit_name);
			world_ptr = (WorldPtr)PyLong_AsVoidPtr(world);
			CApiMatrix4x4 m = matrix4x4_identity();

			if (world_ptr != nullptr && unit_id.nonempty()) {

				if (x_pos != 0.0f || y_pos != 0.0f || z_pos != 0.0f) {
					CApiVector3 v;
					v.x = x_pos;
					v.y = y_pos;
					v.z = z_pos;
					set_translation(m, v);
				}

				if (x_rot != 0.0f || y_rot != 0.0f || z_rot != 0.0f || w_rot != 1.0f) {
					CApiQuaternion q;
					q.x = x_rot;
					q.y = y_rot;
					q.z = z_rot;
					q.w = w_rot;
					CApiMatrix4x4 r = matrix4x4(q);
					set_rotation(m, r);
				}

				UnitRef unit_ptr = PythonPlugin::get_api()._script->World->spawn_unit(world_ptr, unit_id.id(), unit_name, (ConstMatrix4x4Ptr) &m);
				if (unit_ptr != NULL)
				{
					PyObject* python_unit = Py_BuildValue("I", unit_ptr);
					Py_IncRef(python_unit);
					return python_unit;
				}
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonWorld::py_destroy_unit(PyObject* self, PyObject* args)
	{
		
		PyObject* world = Py_None;
		WorldPtr world_ptr = nullptr;
		PyObject* unit = Py_None;
		UnitRef unit_ref = UINT_MAX;

		if (PyArg_ParseTuple(args, "OO", &world, &unit))
		{
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			unit_ref = (UnitRef) PyLong_AsVoidPtr(unit);
			if (world_ptr != nullptr && unit_ref != UINT_MAX) {
				PythonPlugin::get_api()._script->World->destroy_unit(world_ptr, unit_ref);
				Py_RETURN_NONE;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonWorld::py_load_level(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr = nullptr;;
		const char* level_name = "";
		IdString64 level_id;
		IdString64 empty_id;
		unsigned length;

		if (PyArg_ParseTuple(args, "Os#", &world, &level_name, &length))
		{
			level_id = IdString64(length, level_name);
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			if (world_ptr != nullptr && level_id.nonempty()) {
				LevelPtr level_ptr = PythonPlugin::get_api()._script->World->load_level(world_ptr, level_id.id(), level_name, (ConstMatrix4x4Ptr) &matrix4x4_identity(), empty_id.id());
				PyObject* python_level_ptr = PyLong_FromVoidPtr(level_ptr);
				Py_IncRef(python_level_ptr);
				return python_level_ptr;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}


	PyObject* PythonWorld::py_destroy_level(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr = nullptr;
		PyObject* level = Py_None;
		LevelPtr level_ptr = nullptr;

		if (PyArg_ParseTuple(args, "OO", &world, &level))
		{
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			level_ptr = (LevelPtr) PyLong_AsVoidPtr(level);
			if (world_ptr != nullptr && level_ptr != nullptr) {
				PythonPlugin::get_api()._script->World->destroy_level(world_ptr, level_ptr);
				Py_RETURN_NONE;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonWorld::py_world_update(PyObject* self, PyObject* args)
	{
		PyObject* world = Py_None;
		WorldPtr world_ptr = nullptr;
		float delta_time = 0.0;

		if (PyArg_ParseTuple(args, "Of", &world, &delta_time))
		{
			world_ptr = (WorldPtr) PyLong_AsVoidPtr(world);
			if (world_ptr != nullptr) {
				PythonPlugin::get_api()._script->World->update(world_ptr, delta_time);
				Py_RETURN_NONE;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}
}
