#include "python_capture_frame.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef captureframe_methods[] =
	{
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC initcaptureframe(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		app_module = Py_InitModule("stingray.CaptureFrame", captureframe_methods);
		PyModule_AddObject(stingray_module, "CaptureFrame", app_module);
	}
}
