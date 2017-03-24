#include "python_basic.h"

namespace PLUGIN_NAMESPACE
{
	PyObject* PythonBasic::py_write(PyObject* self, PyObject* args)
	{
		const char *what;
		if (!PyArg_ParseTuple(args, "s", &what))
			return NULL;
		PythonPlugin::info(what);
		return Py_BuildValue("");
	}

	PyObject* PythonBasic::py_error(PyObject* self, PyObject* args)
	{
		const char *what;
		if (!PyArg_ParseTuple(args, "s", &what))
			return NULL;
		const char check[] = "\n";
		if (strequal(check, what))
			return NULL;
		PythonPlugin::error(what);
		return Py_BuildValue("");
	}
}


