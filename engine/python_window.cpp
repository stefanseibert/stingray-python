#include "python_window.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef window_methods[] =
	{
		{ "set_title", PythonWindow::py_set_title, METH_VARARGS, "Sets the window title." },
		{ "set_foreground", PythonWindow::py_set_foreground, METH_VARARGS, "Sets window to the foreground." },
		{ "open_window", (PyCFunction)PythonWindow::py_open_window, METH_VARARGS | METH_KEYWORDS, "Opens a new window." },
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC initwindow(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"stingray.Window",
			"Stingray Window Python Module",
			-1,
			window_methods,
			NULL, NULL, NULL, NULL
		};
		app_module = PyModule_Create(&moduledef);
		PyModule_AddObject(stingray_module, "Window", app_module);
		return app_module;
	}

	PyObject* PythonWindow::py_open_window(PyObject* self, PyObject* args, PyObject* keywords)
	{
		const char *keyword_list[] = { "width", "height", "x", "y", "visible", "parent", "title" };
		PyObject *empty = PyTuple_New(0); // pure keyword functions unsupported, but we do not need args
		int width = INT_MAX;
		int height = INT_MAX;
		int x = INT_MAX;
		int y = INT_MAX;
		int visible = INT_MAX;
		int parent = INT_MAX;
		const char* title = "";
		if (PyArg_ParseTupleAndKeywords(empty, keywords, "|iiiiiis", const_cast<char**>(keyword_list), &width,
			&height, &x, &y, &visible, &parent, &title))
		{
			WindowOpenParameter wop;
			PythonPlugin::get_api()._script->Window->fill_default_open_parameter(&wop);
			if (width != INT_MAX)
				wop.width = width;
			if (height != INT_MAX)
				wop.height = height;
			if (x != INT_MAX)
				wop.x = x;
			if (y != INT_MAX)
				wop.y = y;
			if (visible != INT_MAX)
				wop.visible = visible;
			if (parent != INT_MAX)
				wop.optional_parent = (WindowPtr) &parent;
			if (title != "")
				wop.optional_title = title;

			WindowPtr window_ptr = PythonPlugin::get_api()._script->Window->open(&wop);
			PyObject* python_window_ptr = PyLong_FromVoidPtr(window_ptr);
			Py_IncRef(python_window_ptr);
			return python_window_ptr;
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonWindow::py_set_title(PyObject* self, PyObject* args)
	{
		PyObject* window = Py_None;
		WindowPtr window_ptr = nullptr;
		const char* title = "";

		if (PyArg_ParseTuple(args, "s|O", &title, &window))
		{
			if (window != Py_None)
				window_ptr = (WindowPtr) PyLong_AsVoidPtr(window);
			PythonPlugin::get_api()._script->Window->set_title(window_ptr, title);
			Py_RETURN_NONE;
		}
		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonWindow::py_set_foreground(PyObject* self, PyObject* args)
	{
		PyObject* window = Py_None;
		WindowPtr window_ptr = nullptr;

		if (PyArg_ParseTuple(args, "|O", &window))
		{
			if (window != Py_None)
				window_ptr = (WindowPtr)PyLong_AsVoidPtr(window);
			PythonPlugin::get_api()._script->Window->set_foreground(window_ptr);
			Py_RETURN_NONE;
		}
		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}
}
