#include "python_storyteller.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef storyteller_methods[] =
	{
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC initstoryteller(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		app_module = Py_InitModule("stingray.StoryTeller", storyteller_methods);
		PyModule_AddObject(stingray_module, "StoryTeller", app_module);
	}
}
