#include "python_unit.h"

namespace PLUGIN_NAMESPACE
{
	static PyMethodDef unit_methods[] =
	{
		{ "camera", PythonUnit::py_camera, METH_VARARGS, "Retrieves the specified camera owned by the unit." },
		{ "set_local_position", PythonUnit::py_set_local_position, METH_VARARGS, "Sets the local position of a node in the unit." },
		{ "set_local_rotation", PythonUnit::py_set_local_rotation, METH_VARARGS, "Sets the local rotation of a node in the unit." },
		{ "set_local_scale", PythonUnit::py_set_local_scale, METH_VARARGS, "Sets the local scale of a node in the unit." },
		{ "local_position", PythonUnit::py_local_position, METH_VARARGS, "Retrieves the local position of a node in the unit." },
		{ "local_rotation", PythonUnit::py_local_rotation, METH_VARARGS, "Retrieves the local rotation of a node in the unit." },
		{ "local_scale", PythonUnit::py_local_scale, METH_VARARGS, "Retrieves the local scale of a node in the unit." },
		{ "has_node", PythonUnit::py_node, METH_VARARGS, "Indicates whether or not the unit has an object node that matches the specified name." },
		{ "node", PythonUnit::py_node, METH_VARARGS, "Retrieves the index of the object node that matches the specified name." },
		{ NULL, NULL, 0, NULL }        /* Sentinel */
	};

	PyMODINIT_FUNC PyInit_Unit(void)
	{
		PyObject* app_module;
		PyObject* stingray_module = PythonPlugin::get_stingray_module();
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"Unit",
			"Stingray Unit Python Module",
			-1,
			unit_methods,
			NULL, NULL, NULL, NULL
		};
		app_module = PyModule_Create(&moduledef);
		if (app_module == NULL)
			return NULL;
		PyModule_AddObject(stingray_module, "Unit", app_module);
		return app_module;
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

	PyObject* PythonUnit::py_set_local_position(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref = NULL;
		unsigned node_index;
		float x_pos = 0.0f;
		float y_pos = 0.0f;
		float z_pos = 0.0f;

		if (PyArg_ParseTuple(args, "IIfff", &unit, &node_index, &x_pos, &y_pos, &z_pos))
		{
			unit_ref = (UnitRef) unit;
			if(unit_ref != NULL) {
				CApiVector3 v;
				v.x = x_pos;
				v.y = y_pos;
				v.z = z_pos;
				PythonPlugin::get_api()._script->Unit->set_local_position(unit_ref, node_index, &v);
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonUnit::py_set_local_rotation(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref = NULL;
		unsigned node_index;
		float x_rot = 0.0f;
		float y_rot = 0.0f;
		float z_rot = 0.0f;
		float w_rot = 1.0f;

		if (PyArg_ParseTuple(args, "IIffff", &unit, &node_index, &x_rot, &y_rot, &z_rot, &w_rot))
		{
			unit_ref = (UnitRef) unit;
			if (unit_ref != NULL) {
				CApiQuaternion q;
				q.x = x_rot;
				q.y = y_rot;
				q.z = z_rot;
				q.w = w_rot;
				PythonPlugin::get_api()._script->Unit->set_local_rotation(unit_ref, node_index, &q);
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonUnit::py_set_local_scale(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref = NULL;
		unsigned node_index;
		float x_pos = 0.0f;
		float y_pos = 0.0f;
		float z_pos = 0.0f;

		if (PyArg_ParseTuple(args, "IIfff", &unit, &node_index, &x_pos, &y_pos, &z_pos))
		{
			unit_ref = (UnitRef) unit;
			if (unit_ref != NULL) {
				CApiVector3 v;
				v.x = x_pos;
				v.y = y_pos;
				v.z = z_pos;
				PythonPlugin::get_api()._script->Unit->set_local_scale(unit_ref, node_index, &v);
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonUnit::py_local_position(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref = NULL;
		unsigned node_index;

		if (PyArg_ParseTuple(args, "II", &unit, &node_index))
		{
			unit_ref = (UnitRef) unit;
			if (unit_ref != NULL) {
				ConstVector3Ptr v = PythonPlugin::get_api()._script->Unit->local_position(unit_ref, node_index);
				PyObject* result = Py_BuildValue("fff", v->x, v->y, v->z);
				if (result)
					return result;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonUnit::py_local_rotation(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref = NULL;
		unsigned node_index;

		if (PyArg_ParseTuple(args, "II", &unit, &node_index))
		{
			unit_ref = (UnitRef) unit;
			if (unit_ref != NULL) {
				CApiQuaternion q = PythonPlugin::get_api()._script->Unit->local_rotation(unit_ref, node_index);
				PyObject* result = Py_BuildValue("ffff", q.x, q.y, q.z, q.w);
				if (result)
					return result;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonUnit::py_local_scale(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref = NULL;
		unsigned node_index;

		if (PyArg_ParseTuple(args, "II", &unit, &node_index))
		{
			unit_ref = (UnitRef) unit;
			if (unit_ref != NULL) {
				ConstVector3Ptr v = PythonPlugin::get_api()._script->Unit->local_scale(unit_ref, node_index);
				PyObject* result = Py_BuildValue("fff", v->x, v->y, v->z);
				if (result)
					return result;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}

	PyObject* PythonUnit::py_has_node(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref;
		const char* node_name = "";
		IdString32 node_id;
		unsigned length = 0;

		if (PyArg_ParseTuple(args, "Is#", &unit, &node_name, &length))
		{
			unit_ref = (UnitRef) unit;
			if (unit_ref != NULL) {
				node_id = IdString32(length, node_name);
				if (PythonPlugin::get_api()._script->Unit->has_node(unit_ref, node_id.id()))
					return Py_True;
			}
		}

		PythonPlugin::check_exceptions();
		return Py_False;
	}

	PyObject* PythonUnit::py_node(PyObject* self, PyObject* args)
	{
		PyObject* unit = Py_None;
		UnitRef unit_ref;
		const char* node_name = "";
		IdString32 node_id;
		unsigned length = 0;

		if (PyArg_ParseTuple(args, "Is#", &unit, &node_name, &length))
		{
			unit_ref = (UnitRef) unit;
			if (unit_ref != NULL) {
				node_id = IdString32(length, node_name);
				unsigned result_id = PythonPlugin::get_api()._script->Unit->node(unit_ref, node_id.id(), "");

				PyObject* result = Py_BuildValue("I", &result_id);
				if (result)
					return result;
			}
		}

		PythonPlugin::check_exceptions();
		Py_RETURN_NONE;
	}
}
