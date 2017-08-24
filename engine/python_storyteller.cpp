#include "python_storyteller.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef storyteller_methods[] =
	{
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC PyInit_StoryTeller(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"StoryTeller",
			"Stingray StoryTeller Python Module",
			-1,
			storyteller_methods,
			NULL, NULL, NULL, NULL
		};
		app_module = PyModule_Create(&moduledef);
		if (app_module == NULL)
			return NULL;
		PyModule_AddObject(stingray_module, "StoryTeller", app_module);
		return app_module;
	}
}
