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
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"stingray.CaptureFrame",
			"Stingray CaptureFrame Python Module",
			-1,
			captureframe_methods,
			NULL, NULL, NULL, NULL
		};
		app_module = PyModule_Create(&moduledef);
		if (app_module == NULL)
			return NULL;
		PyModule_AddObject(stingray_module, "CaptureFrame", app_module);
		return app_module;
	}
}
