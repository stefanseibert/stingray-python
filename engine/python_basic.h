#pragma once
#include <Python.h>
#include "python_plugin.h"
#include "application/plugin/c_api/c_api_application.h"

namespace PLUGIN_NAMESPACE
{
	class PythonBasic
	{
	public:
		static PyObject* py_write(PyObject* self, PyObject* args);
		static PyObject* py_error(PyObject* self, PyObject* args);
	};
}
