## Python Plugin to use Python 2.7.12 in Stingray

### Installation Steps (Currently only Windows 64bit supported)
- Install the 64bit version of Python 2.7.12 into the standard directory C:\Python27
- Set the system variable PYTHON_INCLUDE_DIRS to C:\Python27\include (or respective correct folder)
- Set the system variable PYTHON_LIBRARIES to C:\Python27\libs (or respective correct folder)
- Set the system variable PYTHONPATH to C:\Python27 (or respective correct folder)
- Add this repository into your stingray source code under "runtime/plugins/"
- Built stingray with "--use-python" (provided you have a stingray branch that supports the python extension)

### Known Problems in Debug Builds
If you cannot build the python plugin in debug, because "python27_d.lib" cannot be found, this is a workound:
- Go into the Python lib Folder (mostly C:/Python27/libs) and copy "python27.lib" to "python27_d.lib"
- Open pyconfig.h (mostly C:/Python27/include/pyconfig.h) and comment out the line "#define Py_DEBUG"
- After this it should be possible to link to Python in Debug Builds
- You could of course also build your own python distribution in Debug if you want to instead

