#pragma once
#include <Python.h>
#include "python_basic.h"

namespace PLUGIN_NAMESPACE
{
	PyMODINIT_FUNC initlevel(void);

	class PythonLevel
	{
	public:
		static PyObject* py_spawn_background(PyObject* self, PyObject* args);
		static PyObject* py_trigger_level_loaded(PyObject* self, PyObject* args);
		static PyObject* py_trigger_level_update(PyObject* self, PyObject* args);
		static PyObject* py_trigger_level_shutdown(PyObject* self, PyObject* args);
	};
}
