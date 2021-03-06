#pragma once
#include "python_basic.h"

namespace PLUGIN_NAMESPACE
{
	PyMODINIT_FUNC PyInit_Window(void);

	class PythonWindow
	{
	public:
		static PyObject* py_open_window(PyObject* self, PyObject* args, PyObject* keywords);
		static PyObject* py_set_title(PyObject* self, PyObject* args);
		static PyObject* py_set_foreground(PyObject* self, PyObject* args);
	};
}
