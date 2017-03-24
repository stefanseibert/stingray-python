## Plugin to use Python 2.7.12 in Stingray

Stingray is a real-time rendering engine from Autodesk:
http://www.stingrayengine.com

Installation Steps (only Windows 64bit currently):  
Install Python 2.7.12 64bit on your disk, referred as [YOUR_INSTALL_PATH]  
Set the system variable PYTHON_INCLUDE_DIRS to [YOUR_INSTALL_PATH]\include  
Set the system variable PYTHON_LIBRARIES to [YOUR_INSTALL_PATH]\libs  
Set the system variable PYTHONPATH to [YOUR_INSTALL_PATH]

## Known Problems in Debug Builds

If you wanna debug the plugin yourself and cannot get it run, this is because your python  
installation is by default not installed as debug version. Either you get the python sources  
yourself and build it in debug or use this workound:

Go into the python lib Folder ([YOUR_INSTALL_PATH]/libs) and copy "python27.lib" to "python27_d.lib"  
Open pyconfig.h ([YOUR_INSTALL_PATH]/include/pyconfig.h) and comment out the line "#define Py_DEBUG"  
After this it should be possible to link to Python in Debug Builds

If you are new with Stingray plugins you can find all  
needed information on the official Stingray plugin page:  
https://github.com/AutodeskGames/stingray-plugin

## Additional information?

The example project of this plugin contains example scripts how to hook your python code into Stingray

## Important Notes

This is a free time / hackday based project of one of the Stingray developers and not under official  
release from Autodesk. I try to add features to the Python API from time to time but using this code  
is at own risk and you should always backup your project files beforehand.
