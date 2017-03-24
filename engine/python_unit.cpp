#include "python_unit.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef unit_methods[] =
	{
		{ "camera", PythonUnit::py_camera, METH_VARARGS, "Retrieves the specified camera owned by the unit." },
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC initunit(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		app_module = Py_InitModule("stingray.Unit", unit_methods);
		PyModule_AddObject(stingray_module, "Unit", app_module);
	}

	PyObject* PythonUnit::py_camera(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref;
		const char* camera_name = "";
		IdString32 camera_id;
		unsigned length = 0;

		if (PyArg_ParseTuple(args, "Is#", &unit, &camera_name, &length))
		{
			unit_ref = (UnitRef) unit;
			camera_id = IdString32(length, camera_name);
			if (unit_ref != NULL && length > 0 ) {
				unsigned index = PythonPlugin::get_api()._script->Unit->find_camera(unit_ref, camera_id.id());
				CameraPtr camera_ptr = PythonPlugin::get_api()._script->Unit->camera(unit_ref, index, camera_name);
				PyObject* python_camera_ptr = PyLong_FromVoidPtr(camera_ptr);
				Py_IncRef(python_camera_ptr);
				return python_camera_ptr;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}
}
