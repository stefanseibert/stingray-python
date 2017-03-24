#pragma once
#include "python_basic.h"
#include "plugin_foundation/id_string.h"
#include "plugin_foundation/vector.h"
#include "plugin_foundation/matrix4x4.h"

namespace PLUGIN_NAMESPACE
{
	PyMODINIT_FUNC initworld(void);

	class PythonWorld
	{
	public:
		static PyObject* py_create_shading_environment(PyObject* self, PyObject* args);
		static PyObject* py_create_default_shading_environment(PyObject* self, PyObject* args);
		static PyObject* py_destroy_shading_environment(PyObject* self, PyObject* args);
		static PyObject* py_spawn_unit(PyObject* self, PyObject* args);
		static PyObject* py_destroy_unit(PyObject* self, PyObject* args);
		static PyObject* py_load_level(PyObject* self, PyObject* args);
		static PyObject* py_destroy_level(PyObject* self, PyObject* args);
		static PyObject* py_world_update(PyObject* self, PyObject* args);
	};
}
