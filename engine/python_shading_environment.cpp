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
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"stingray.ShadingEnvironment",
			"Stingray ShadingEnvironment Python Module",
			-1,
			shadingenvironment_methods,
			NULL, NULL, NULL, NULL
		};
		app_module = PyModule_Create(&moduledef);
		if (app_module == NULL)
			return NULL;
		PyModule_AddObject(stingray_module, "ShadingEnvironment", app_module);
		return app_module;
	}
}
