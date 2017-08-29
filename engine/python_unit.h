#pragma once
#include "python_basic.h"

namespace PLUGIN_NAMESPACE
{
	PyMODINIT_FUNC PyInit_Unit(void);

	class PythonUnit
	{
	public:
		static PyObject* py_camera(PyObject* self, PyObject* args);
		static PyObject* py_set_local_position(PyObject* self, PyObject* args);
		static PyObject* py_set_local_rotation(PyObject* self, PyObject* args);
		static PyObject* py_set_local_scale(PyObject* self, PyObject* args);
		static PyObject* py_local_position(PyObject* self, PyObject* args);
		static PyObject* py_local_rotation(PyObject* self, PyObject* args);
		static PyObject* py_local_scale(PyObject* self, PyObject* args);
		static PyObject* py_has_node(PyObject* self, PyObject* args);
		static PyObject* py_node(PyObject* self, PyObject* args);
	};
}
