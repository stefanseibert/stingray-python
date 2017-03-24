#pragma once
#include <Python.h>
#include "python_basic.h"
#include "application/rendering/graph/compile_data.h"

namespace PLUGIN_NAMESPACE
{
	PyMODINIT_FUNC initapplication(void);

	class PythonApplication
	{
	public:
		static PyObject* py_argv(PyObject* self, PyObject* args);
		static PyObject* py_new_world(PyObject* self, PyObject* args, PyObject* keywords);
		static PyObject* py_release_world(PyObject* self, PyObject* args);
		static PyObject* py_create_viewport(PyObject* self, PyObject* args);
		static PyObject* py_destroy_viewport(PyObject* self, PyObject* args);
		static PyObject* py_main_world(PyObject* self, PyObject* args);
		static PyObject* py_can_get(PyObject* self, PyObject* args);
		static PyObject* py_quit(PyObject* self, PyObject* args);
		static PyObject* py_render_world(PyObject* self, PyObject* args);
		/*static PyObject* py_autoload_resource_package(PyObject* self, PyObject* args);*/
	};
}
