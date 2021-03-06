#pragma once
#include <Python.h>
#include "python_plugin.h"

namespace PLUGIN_NAMESPACE
{
	class PythonBasic
	{
	public:
		static PyObject* py_write(PyObject* self, PyObject* args);
		static PyObject* py_error(PyObject* self, PyObject* args);
	};
}
