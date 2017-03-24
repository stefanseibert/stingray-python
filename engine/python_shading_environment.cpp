#include "python_shading_environment.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef shadingenvironment_methods[] =
	{
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC initshadingenvironment(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		app_module = Py_InitModule("stingray.ShadingEnvironment", shadingenvironment_methods);
		PyModule_AddObject(stingray_module, "ShadingEnvironment", app_module);
	}
}
