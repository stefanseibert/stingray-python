#include <engine_plugin_api/plugin_api.h>
#include <plugin_foundation/platform.h>
#include <python_plugin.h>

extern "C"
{
	#ifdef STATIC_LINKING
		void *get_python_plugin_api(unsigned api)
		{
			if (api == PLUGIN_API_ID) {
				static struct PluginApi api = {0};
				api.setup_game = &PLUGIN_NAMESPACE::PythonPlugin::setup_game;
				api.update_game = &PLUGIN_NAMESPACE::PythonPlugin::update_game;
				api.shutdown_game = &PLUGIN_NAMESPACE::PythonPlugin::shutdown_game;
				api.setup_data_compiler = &PLUGIN_NAMESPACE::PythonPlugin::setup_compiler;
				api.shutdown_data_compiler = &PLUGIN_NAMESPACE::PythonPlugin::shutdown_data_compiler;
				api.setup_resources = &PLUGIN_NAMESPACE::PythonPlugin::setup_resources;
				api.shutdown_resources = &PLUGIN_NAMESPACE::PythonPlugin::shutdown_resources;
				api.can_refresh = &PLUGIN_NAMESPACE::PythonPlugin::can_refresh;
				api.refresh = &PLUGIN_NAMESPACE::PythonPlugin::refresh;
				return &api;
			}
			return 0;
		}
	#else
		PLUGIN_DLLEXPORT void *get_plugin_api(unsigned api)
		{
			if (api == PLUGIN_API_ID) {
				static struct PluginApi api = {0};
				api.setup_game = &PLUGIN_NAMESPACE::PythonPlugin::setup_game;
				api.update_game = &PLUGIN_NAMESPACE::PythonPlugin::update_game;
				api.shutdown_game = &PLUGIN_NAMESPACE::PythonPlugin::shutdown_game;
				api.setup_data_compiler = &PLUGIN_NAMESPACE::PythonPlugin::setup_compiler;
				api.shutdown_data_compiler = &PLUGIN_NAMESPACE::PythonPlugin::shutdown_data_compiler;
				api.setup_resources = &PLUGIN_NAMESPACE::PythonPlugin::setup_resources;
				api.shutdown_resources = &PLUGIN_NAMESPACE::PythonPlugin::shutdown_resources;
				api.can_refresh = &PLUGIN_NAMESPACE::PythonPlugin::can_refresh;
				api.refresh = &PLUGIN_NAMESPACE::PythonPlugin::refresh;
				return &api;
			}
			return 0;
		}
	#endif
}
