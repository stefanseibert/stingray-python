#pragma once
#include "Windows.h"
#include <engine_plugin_api/plugin_api.h>
#include <plugin_foundation/string.h>
#include "plugin_foundation/hash_function.h"
#include <plugin_foundation/stream.h>
#include <Python.h>
#include "python_basic.h"
#include "python_application.h"
#include "python_capture_frame.h"
#include "python_keyboard.h"
#include "python_level.h"
#include "python_shading_environment.h"
#include "python_storyteller.h"
#include "python_unit.h"
#include "python_window.h"
#include "python_world.h"

// Hook to enable waiting for the Debugger to debug python init code
// Uncommenting this define lets the plugin wait until VS debugger is connected
// #define WAITFORDEBUGGER

namespace PLUGIN_NAMESPACE
{
	using namespace stingray_plugin_foundation;

	struct ApiInterface
	{
		LoggingApi *_logging;
		FileSystemApi *_file_system;
		LuaApi *_lua;
		RenderBufferApi *_render_buffer;
		StreamCaptureApi *_capture;
		DataCompilerApi *_data_compiler;
		DataCompileParametersApi *_data_compile_parameters;
		AllocatorApi *_allocator;
		AllocatorObject *_allocator_object;
		ResourceManagerApi *_resource_manager;
		ApplicationApi *_application;
		ScriptApi *_script;
	};

	class PythonPlugin
	{
	private:
		static PyObject* _stingray_module;
	public:
		static ApiInterface &get_api();
		static ApiAllocator &get_allocator();
		static PyObject* get_stingray_module();
		static void set_stingray_module(PyObject* module);
		static void setup_game(GetApiFunction get_engine_api);
		static void update_game(float dt);
		static void render_game();
		static bool check_exceptions();
		static void on_python_setup();
		static void on_python_update(float dt);
		static void on_python_render();
		static void on_python_shutdown();
		static void shutdown_game();
		static void setup_resources(GetApiFunction get_engine_api);
		static void shutdown_resources();
		static void setup_compiler(GetApiFunction get_engine_api);
		static void shutdown_data_compiler();
		static int can_refresh(uint64_t type);
		static void refresh(uint64_t type, uint64_t name);
		static DataCompileResult compile_python(DataCompileParameters *input);
		static const char *get_name();
		static void error(const char* type, const char* message);
		static void error(const char* message);
		static void info(const char* message);
		static PyObject* register_setup(PyObject* self, PyObject* args);
		static PyObject* register_update(PyObject* self, PyObject* args);
		static PyObject* register_render(PyObject* self, PyObject* args);
		static PyObject* register_shutdown(PyObject* self, PyObject* args);
	};
}
