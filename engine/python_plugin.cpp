#include "python_plugin.h"


namespace PLUGIN_NAMESPACE
{
	using namespace stingray_plugin_foundation;

	PyObject* PythonPlugin::_stingray_module = nullptr;

	namespace
	{
		static bool _compiler_api_initialized = false;
		static bool _game_api_initialized = false;
		static const char* PYTHON_EXTENSION = "py";
		static const char* PYTHON_BOOT_FILE = "python-init";
		static uint64_t python_type_id = hash64(PYTHON_EXTENSION);
		static uint64_t python_boot_file_id = hash64(PYTHON_BOOT_FILE);
	}

#define WAITFORDEBUGGER

	void wait_for_debugger()
	{
	#ifdef WAITFORDEBUGGER
		while( !IsDebuggerPresent() )
				Sleep( 100 ); 
	#endif
	}

	ApiInterface _api;
	ApiInterface& PythonPlugin::get_api()
	{
		return _api;
	}

	ApiAllocator _python_allocator = ApiAllocator(nullptr, nullptr);
	ApiAllocator& PythonPlugin::get_allocator()
	{
		return _python_allocator;
	}

	void init_compiler_api(GetApiFunction get_engine_api)
	{
		_api._data_compiler = (DataCompilerApi *)get_engine_api(DATA_COMPILER_API_ID);
		_api._data_compile_parameters = (DataCompileParametersApi *)get_engine_api(DATA_COMPILE_PARAMETERS_API_ID);
		_api._resource_manager = (ResourceManagerApi *)get_engine_api(RESOURCE_MANAGER_API_ID);
		_api._application = (ApplicationApi *)get_engine_api(APPLICATION_API_ID);
		_api._allocator = (AllocatorApi *)get_engine_api(ALLOCATOR_API_ID);
		_api._allocator_object = _api._allocator->make_plugin_allocator("PythonPlugin");
		_python_allocator = ApiAllocator(_api._allocator, _api._allocator_object);
		_compiler_api_initialized = true;
	}

	void init_game_api(GetApiFunction get_engine_api)
	{
		_api._render_buffer = (RenderBufferApi*)get_engine_api(RENDER_BUFFER_API_ID);
		_api._capture = (StreamCaptureApi*)get_engine_api(STREAM_CAPTURE_API_ID);
		_api._script = (ScriptApi *)get_engine_api(C_API_ID);
		_api._lua = (LuaApi *)get_engine_api(LUA_API_ID);
		_api._logging = (LoggingApi*)get_engine_api(LOGGING_API_ID);
		_api._file_system = (FileSystemApi *)get_engine_api(FILESYSTEM_API_ID);
		_api._resource_manager = (ResourceManagerApi *)get_engine_api(RESOURCE_MANAGER_API_ID);
		_api._allocator = (AllocatorApi *)get_engine_api(ALLOCATOR_API_ID);
		_api._allocator_object = _api._allocator->make_plugin_allocator("PythonPlugin");
		_python_allocator = ApiAllocator(_api._allocator, _api._allocator_object);
		_game_api_initialized = true;
	}

	void deinit_game_api()
	{
		_api._allocator->destroy_plugin_allocator(_api._allocator_object);
		_api._allocator = nullptr;
		_api._render_buffer = nullptr;
		_api._capture = nullptr;
		_api._script = nullptr;
		_api._lua = nullptr;
		_api._logging = nullptr;
		_api._file_system = nullptr;
		_api._resource_manager = nullptr;
		_game_api_initialized = false;
	}

	void deinit_compiler_api()
	{
		_api._allocator->destroy_plugin_allocator(_api._allocator_object);
		_api._data_compiler = nullptr;
		_api._data_compile_parameters = nullptr;
		_api._resource_manager = nullptr;
		_api._application = nullptr;
		_api._allocator = nullptr;
		_compiler_api_initialized = false;
	}

	struct PythonSession
	{
		Allocator &allocator;
		Array<PyObject*> start_callbacks;
		Array<PyObject*> update_callbacks;
		Array<PyObject*> render_callbacks;
		Array<PyObject*> end_callbacks;

		PythonSession(Allocator &a) :
			allocator(a),
			start_callbacks(a),
			update_callbacks(a),
			render_callbacks(a),
			end_callbacks(a) { }

		~PythonSession() {}
	};

	static PythonSession *python_session = nullptr;

	void PythonPlugin::on_python_update(float dt)
	{
		PyObject *arglist = Py_BuildValue("(f)", dt);
		for(unsigned i = 0; i < python_session->update_callbacks.size(); i++)
		{
			PyEval_CallObject(python_session->update_callbacks[i], arglist);
			if (check_exceptions())
			{
				PyObject* last = python_session->update_callbacks.back();
				python_session->update_callbacks[i] = last;
				python_session->update_callbacks.pop_back();
			}
		}
		Py_DecRef(arglist);
	}

	void PythonPlugin::on_python_render()
	{
		PyObject *arglist = Py_BuildValue("()");
		for (unsigned i = 0; i < python_session->render_callbacks.size(); i++)
		{
			PyEval_CallObject(python_session->render_callbacks[i], arglist);
			if (check_exceptions())
			{
				PyObject* last = python_session->render_callbacks.back();
				python_session->render_callbacks[i] = last;
				python_session->render_callbacks.pop_back();
			}
		}
		Py_DecRef(arglist);
	}

	void PythonPlugin::on_python_setup()
	{
		PyObject *arglist = Py_BuildValue("()");
		for (unsigned i = 0; i < python_session->start_callbacks.size(); i++)
		{
			PyEval_CallObject(python_session->start_callbacks[i], arglist);
			check_exceptions();
		}
		Py_DecRef(arglist);
	}

	void PythonPlugin::on_python_shutdown()
	{
		PyObject *arglist = Py_BuildValue("()");
		for (unsigned i = 0; i < python_session->end_callbacks.size(); i++)
		{
			PyEval_CallObject(python_session->end_callbacks[i], arglist);
			check_exceptions();
		}
		Py_DecRef(arglist);
	}

	bool PythonPlugin::check_exceptions()
	{
		if (PyErr_Occurred())
		{
			PyObject* error_type;
			PyObject* error_value;
			PyObject* error_callstack;
			PyErr_Fetch(&error_type, &error_value, &error_callstack);
			PyObject* str_error_type = PyObject_Repr(error_type);
			PyObject* pyString = PyUnicode_AsEncodedString(str_error_type, "utf-8", "Error ~");

			const char *errorString = PyBytes_AS_STRING(pyString);
			error(errorString);

			if(error_type)
				Py_XDECREF(error_type);
			if (error_value)
				Py_XDECREF(error_value);
			if (error_callstack)
				Py_XDECREF(error_callstack);
			if (str_error_type)
				Py_XDECREF(str_error_type);
			if (pyString)
				Py_XDECREF(pyString);

			PyErr_Clear();
			return true;
		}
		return false;
	}

	PyObject* PythonPlugin::register_update(PyObject *obj, PyObject *args)
	{
		PyObject *python_callback;
		if (!PyArg_Parse(args, "(O)", &python_callback))
			return NULL;
		if (!PyCallable_Check(python_callback))
			PyErr_SetString(PyExc_TypeError, "Register Update: Need a callable function!");
		else {
			python_session->update_callbacks.push_back(python_callback);
		}
		Py_RETURN_NONE;
	}

	PyObject* PythonPlugin::register_render(PyObject *obj, PyObject *args)
	{
		PyObject *python_callback;
		if (!PyArg_Parse(args, "(O)", &python_callback))
			return NULL;
		if (!PyCallable_Check(python_callback))
			PyErr_SetString(PyExc_TypeError, "Register Render: Need a callable function!");
		else {
			python_session->render_callbacks.push_back(python_callback);
		}
		Py_RETURN_NONE;
	}

	PyObject* PythonPlugin::register_setup(PyObject *obj, PyObject *args)
	{
		PyObject *python_callback;
		if (!PyArg_Parse(args, "(O)", &python_callback))
			return NULL;
		if (!PyCallable_Check(python_callback))
			PyErr_SetString(PyExc_TypeError, "Register Setup: Need a callable function!");
		else {
			python_session->start_callbacks.push_back(python_callback);
		}
		Py_RETURN_NONE;
	}

	PyObject* PythonPlugin::register_shutdown(PyObject *obj, PyObject *args)
	{
		PyObject *python_callback;
		if (!PyArg_Parse(args, "(O)", &python_callback))
			return NULL;
		if (!PyCallable_Check(python_callback))
			PyErr_SetString(PyExc_TypeError, "Register Shutdown: Need a callable function!");
		else {
			python_session->end_callbacks.push_back(python_callback);
		}
		Py_RETURN_NONE;
	}

	DataCompileResult PythonPlugin::compile_python(DataCompileParameters *input)
	{
		DataCompileResult python = _api._data_compile_parameters->read(input);
		if (python.error)
			return python;

		const unsigned size = python.data.len + sizeof(unsigned) + 1;
		auto* result = (char*)_api._allocator->allocate(_api._data_compile_parameters->allocator(input), size, 4);

		auto* p = result;
		stream::pack(p, size);
		stream::pack_bytes(p, python.data.p, python.data.len + 1);
		auto blob = DataCompileResult();
		blob.data.p = result;
		blob.data.len = size;
		return blob;
	}

	static PyMethodDef stingray_methods[] =
	{
		{"register_setup",		PythonPlugin::register_setup,		METH_VARARGS, "Registers a setup callback." },
		{"register_update",		PythonPlugin::register_update,		METH_VARARGS, "Registers an update callback." },
		{"register_render",		PythonPlugin::register_render,		METH_VARARGS, "Registers a render callback." },
		{"register_shutdown",	PythonPlugin::register_shutdown,	METH_VARARGS, "Registers a shutdown callback." },
		{NULL, NULL, 0, NULL}        /* Sentinel */
	};

	static PyMethodDef write_methods[] =
	{
		{"write", PythonBasic::py_write, METH_VARARGS, "Write Output to the Stingray Console."},
		{NULL, NULL, 0, NULL}        /* Sentinel */
	};

	static PyMethodDef error_methods[] =
	{
		{"write", PythonBasic::py_error, METH_VARARGS, "Write Errors to the Stingray Console."},
		{NULL, NULL, 0, NULL}        /* Sentinel */
	};

	PyMODINIT_FUNC PyInit_stingray(void)
	{
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"stingray",
			"Stingray Python Module",
			-1,
			stingray_methods,
			NULL, NULL, NULL, NULL
		};
		PyObject* module = PyModule_Create(&moduledef);
		PythonPlugin::set_stingray_module(module);
		return module;
	}

	PyMODINIT_FUNC PyInit_write(void)
	{
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"write",
			"Stingray Write Output Module",
			-1,
			write_methods,
			NULL, NULL, NULL, NULL
		};
		PyObject* module = PyModule_Create(&moduledef);
		PySys_SetObject("stdout", module);
		return module;
	}

	PyMODINIT_FUNC PyInit_error(void)
	{
		static struct PyModuleDef moduledef = {
			PyModuleDef_HEAD_INIT,
			"error",
			"Stingray Error Output Module",
			-1,
			error_methods,
			NULL, NULL, NULL, NULL
		};
		PyObject* module = PyModule_Create(&moduledef);
		PySys_SetObject("stderr", module);
		return module;
	}

	void PythonPlugin::setup_game(GetApiFunction get_engine_api)
	{
		if (!_game_api_initialized)
			init_game_api(get_engine_api);

		wait_for_debugger();

		// Initialize Callback Data Structures
		python_session = MAKE_NEW(get_allocator(), PythonSession, get_allocator());
		PythonSession &p = *python_session;

		// Initialize Python Interpreter and Main Stingray Module
		wchar_t *application_name = L"Stingray";
		PyImport_AppendInittab("stingray", PyInit_stingray);
		
		Py_SetProgramName(application_name);
		Py_Initialize();
		PyInit_write();
		PyInit_error();
		PyImport_ImportModule("stingray");


		// Initialize Built In Python Modules
		PyInit_Application();
		PyInit_CaptureFrame();
		PyInit_Keyboard();
		PyInit_Level();
		PyInit_ShadingEnvironment();
		PyInit_StoryTeller();
		PyInit_Unit();
		PyInit_Window();
		PyInit_World();

		// Initialize Custom Python Modules
		// ...

		// Call Boot Script
		if (_api._resource_manager->can_get(PYTHON_EXTENSION, PYTHON_BOOT_FILE)) {
			auto* data = (char*)_api._resource_manager->get(PYTHON_EXTENSION, PYTHON_BOOT_FILE);
			unsigned len = stream::unpack<unsigned>(data);
			PyRun_SimpleString(data);
		}
		else
			error("Could not find: python-init.py. No Python Code was initialized.");

		// Call Engine Setup Step
		PyGILState_STATE gstate;
		gstate = PyGILState_Ensure();
		on_python_setup();
		PyGILState_Release(gstate);
	}

	void PythonPlugin::update_game(float dt)
	{
		PyGILState_STATE gstate;
		gstate = PyGILState_Ensure();
		on_python_update(dt);

		// TODO: Plugin API misses render step, should be fixed at some point
		on_python_render();
		PyGILState_Release(gstate);
	}

	void PythonPlugin::render_game()
	{
		PyGILState_STATE gstate;
		gstate = PyGILState_Ensure();
		on_python_render();
		PyGILState_Release(gstate);
	}

	void PythonPlugin::shutdown_game()
	{
		PyGILState_STATE gstate;
		gstate = PyGILState_Ensure();
		on_python_shutdown();
		PyGILState_Release(gstate);
		Py_Finalize();
		make_delete(get_allocator(), python_session);
		deinit_game_api();
	}

	PyObject* PythonPlugin::get_stingray_module()
	{
		return _stingray_module;
	}

	void PythonPlugin::set_stingray_module(PyObject* module)
	{
		_stingray_module = module;
	}

	void PythonPlugin::setup_resources(GetApiFunction get_engine_api)
	{
		if(!_compiler_api_initialized)
			init_compiler_api(get_engine_api);

		_api._resource_manager->register_type(PYTHON_EXTENSION);
	}

	void PythonPlugin::setup_compiler(GetApiFunction get_engine_api)
	{
		if(!_compiler_api_initialized)
			init_compiler_api(get_engine_api);

		DataCompilerApi *compiler = (DataCompilerApi *)get_engine_api(DATA_COMPILER_API_ID);
		compiler->add_compiler(PYTHON_EXTENSION, 1, PythonPlugin::compile_python);
	}

	void PythonPlugin::shutdown_data_compiler()
	{
		deinit_compiler_api();
	}

	void PythonPlugin::shutdown_resources()
	{
		// Nothing to do right now
	}

	int PythonPlugin::can_refresh(uint64_t type)
	{
		return type == python_type_id;
	}

	void PythonPlugin::refresh(uint64_t type, uint64_t name)
	{
		if (name == python_boot_file_id && _api._resource_manager->can_get_by_id(type, python_boot_file_id) && python_session) {
			python_session->start_callbacks.clear();
			python_session->update_callbacks.clear();
			python_session->render_callbacks.clear();
			python_session->end_callbacks.clear();
			auto* data = (char*)_api._resource_manager->get_by_id(type, python_boot_file_id);
			unsigned len = stream::unpack<unsigned>(data);
			PyRun_SimpleString(data);
			return;
		}

		if (type == python_type_id && _api._resource_manager->can_get_by_id(type, name) && python_session) {
			auto* data = (char*)_api._resource_manager->get_by_id(type, name);
			unsigned len = stream::unpack<unsigned>(data);
			PyRun_SimpleString(data);
		}
	}

	const char *PythonPlugin::get_name()
	{
		return "Python";
	}

	void PythonPlugin::error(const char* message)
	{
		_api._logging->error(get_name(), message);
	}

	void PythonPlugin::info(const char* message)
	{
		_api._logging->info(get_name(), message);
	}
}
