#pragma once
#include "python_basic.h"

namespace PLUGIN_NAMESPACE
{
	PyMODINIT_FUNC initunit(void);

	class PythonUnit
	{
	public:
		static PyObject* py_camera(PyObject* self, PyObject* args);
	};
}
