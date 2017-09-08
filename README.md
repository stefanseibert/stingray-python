## Plugin to use Python 3.5 in the Stingray Component

Installation Steps (only Windows 64bit currently):  
Install Python 3.5 64bit on your disk, referred as [YOUR_INSTALL_PATH]  
Set the system variable PYTHON_INCLUDE_DIRS to [YOUR_INSTALL_PATH]\include  
Set the system variable PYTHON_LIBRARIES to [YOUR_INSTALL_PATH]\libs  
Set the system variable PYTHONPATH to [YOUR_INSTALL_PATH]

Having these environment variables set should enable you  
to build the plugin by issuing "make" from the console.

If you are new with Stingray plugins you can find all  
needed information on the official Stingray plugin page:  
https://github.com/AutodeskGames/stingray-plugin
(There is also an step by step installation guide and Wiki)''

## Versioning
From V2.0 on this tool follows the versioning guidelines from Stingray. This means that the
release/2.0.0 branch targets Stingray V2.0 and develop targets the current develop remote branch.

## Known Problems in Debug Builds

If you wanna debug the plugin yourself and cannot get it run, this is because your python  
installation is by default not installed as debug version. Either you get the python sources  
yourself and build it in debug or use this workound:

Go into the python lib Folder ([YOUR_INSTALL_PATH]/libs) and copy "python35.lib" to "python35_d.lib"  
Open pyconfig.h ([YOUR_INSTALL_PATH]/include/pyconfig.h) and comment out the line "#define Py_DEBUG"  
After this it should be possible to link to Python in Debug Builds

## Additional information?

The example project of this plugin contains example scripts how to hook your python code into Stingray

## Important Notes

This is a free time / hackday based project of one of the Stingray developers and not under official  
release from Autodesk. I try to add features to the Python API from time to time but using this code  
is at own risk and you should always backup your project files beforehand.

## Known Problems in Debug Builds

If you wanna debug the plugin yourself under windows and cannot get it run, this is because your python  
installation is by default not installed with libraries in debug. Either you get the python sources  
yourself and build them in debug or use this workaround:

Go into the python lib Folder ([YOUR_INSTALL_PATH]/libs) and copy "python27.lib" to "python27_d.lib"  
Open pyconfig.h ([YOUR_INSTALL_PATH]/include/pyconfig.h) and comment out the line "#define Py_DEBUG"  
After this it should be possible to link to Python in Debug Builds
