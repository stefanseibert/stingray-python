#include "python_keyboard.h"


namespace PLUGIN_NAMESPACE
{
	static PyMethodDef keyboard_methods[] =
	{
		{ "keystrokes", PythonKeyboard::py_keystrokes, METH_VARARGS, "Returns a table that contains an entry for each key the user pressed in the previous frame." },
		{ "button", PythonKeyboard::py_button, METH_VARARGS, "Returns the current input value of a button." },
		{ "pressed", PythonKeyboard::py_pressed, METH_VARARGS, "Returns if a certain key was pressed." },
		{ "released", PythonKeyboard::py_released, METH_VARARGS, "Returns if a certain key was released." },
		{ "button_id", PythonKeyboard::py_button_id, METH_VARARGS, "Returns the id of a button." },
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC PyInit_Keyboard(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"Keyboard",
			"Stingray Keyboard Python Module",
			-1,
			keyboard_methods,
			NULL, NULL, NULL, NULL
		};
		app_module = PyModule_Create(&moduledef);
		if (app_module == NULL)
			return NULL;
		PyModule_AddObject(stingray_module, "Keyboard", app_module);
		return app_module;
	}

	PyObject* PythonKeyboard::py_keystrokes(PyObject* self, PyObject* args)
	{
		unsigned num_keystrokes = 0;
		auto keyboard_device = (KeyboardCApi*) PythonPlugin::get_api()._script->Input->Keyboard;
		const int* strokes = keyboard_device->keystrokes(PythonPlugin::get_api()._script->Input->keyboard(), &num_keystrokes);

		PyObject *stroke_list = PyList_New(0);
		for (unsigned i = 0; i < num_keystrokes; ++i)
		{
			int stroke = strokes[i];
			if (stroke < 32) {
				PyObject* py_stroke = Py_BuildValue("I", stroke);
				Py_IncRef(py_stroke);
				PyList_Append(stroke_list, py_stroke);
			}
			//else {
			//	char utf8[5];
			//	stingray_plugin_foundation::encoding::utf8_encode(stroke, utf8);
			//	PyObject* py_stroke = Py_BuildValue("s", utf8);
			//	Py_IncRef(py_stroke);
			//	PyList_Append(stroke_list, py_stroke);
			//}
			PythonPlugin::check_exceptions();
		}
		Py_IncRef(stroke_list);
		return stroke_list;
	}

	PyObject* PythonKeyboard::py_button(PyObject* self, PyObject* args)
	{
		unsigned button_id;

		if (PyArg_ParseTuple(args, "I", &button_id))
		{
			auto keyboard_device = (InputControllerCApi*) PythonPlugin::get_api()._script->Input->InputController;
			int button_value = keyboard_device->button(PythonPlugin::get_api()._script->Input->keyboard(), button_id);

			PyObject *python_button_value = Py_BuildValue("I", button_value);
			Py_IncRef(python_button_value);
			return python_button_value;
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonKeyboard::py_pressed(PyObject* self, PyObject* args)
	{
		unsigned button_id;

		if (PyArg_ParseTuple(args, "I", &button_id))
		{
			auto keyboard_device = (InputControllerCApi*) PythonPlugin::get_api()._script->Input->InputController;
			int pressed_status = keyboard_device->pressed(PythonPlugin::get_api()._script->Input->keyboard(), button_id);

			PyObject *python_pressed_status = Py_BuildValue("I", pressed_status);
			Py_IncRef(python_pressed_status);
			return python_pressed_status;
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonKeyboard::py_released(PyObject* self, PyObject* args)
	{
		unsigned button_id;

		if (PyArg_ParseTuple(args, "I", &button_id))
		{
			auto keyboard_device = (InputControllerCApi*) PythonPlugin::get_api()._script->Input->InputController;
			int released_status = keyboard_device->released(PythonPlugin::get_api()._script->Input->keyboard(), button_id);

			PyObject *python_released_status = Py_BuildValue("I", released_status);
			Py_IncRef(python_released_status);
			return python_released_status;
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonKeyboard::py_button_id(PyObject* self, PyObject* args)
	{
		const char* button_name = "";
		IdString32 button_id32;
		unsigned length;

		if (PyArg_ParseTuple(args, "s#", &button_name, &length))
		{
			button_id32 = IdString32(length, button_name);
			auto keyboard_device = (InputControllerCApi*) PythonPlugin::get_api()._script->Input->InputController;
			unsigned button_id = keyboard_device->button_id(PythonPlugin::get_api()._script->Input->keyboard(), button_id32.id());

			if (button_id != UINT_MAX) {
				PyObject *python_button_id = Py_BuildValue("I", button_id);
				Py_IncRef(python_button_id);
				return python_button_id;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}
}
