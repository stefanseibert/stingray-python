#pragma once
#include <Python.h>
#include "python_basic.h"
#include "engine_plugin_api\c_api\c_api_input_controller.h"
#include "engine_plugin_api\c_api\c_api_keyboard.h"
#include "plugin_foundation/encoding.h"

namespace PLUGIN_NAMESPACE
{
	PyMODINIT_FUNC initkeyboard(void);

	class PythonKeyboard
	{
	public:
		static PyObject* py_keystrokes(PyObject* self, PyObject* args);
		static PyObject* py_button(PyObject* self, PyObject* args);
		static PyObject* py_pressed(PyObject* self, PyObject* args);
		static PyObject* py_released(PyObject* self, PyObject* args);
		static PyObject* py_button_id(PyObject* self, PyObject* args);
	};
}
