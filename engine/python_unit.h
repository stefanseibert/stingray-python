#pragma once
#include "python_basic.h"

namespace PLUGIN_NAMESPACE
{
	PyMODINIT_FUNC PyInit_Unit(void);

	class PythonUnit
	{
	public:
		static PyObject* py_camera(PyObject* self, PyObject* args);
	};
}
