# yba::plugin

> This is a WIP project

A simple and safe C++ plugin system as a header only library

## priciple

Givein a program (called Host program) that want to load code from a dynamic library (called Plugin library) without any build time dependancy beside an interface definition, this library permit you to do so, really simply by doing the following:

Host program instanciate a `plugin_manager` object

Host program and Plugin library share an interface that derive from `plugin`

Plugin library implements the interface mentionned above, and give a *single exported function* that create an object of that type on the heap and return a pointer.

`plugin_manager` load the library via it's name in an os defined way. It then can access that function and create a `plugin` object from it, and automatically manages it's lifetime. It returns to you a plugin object that you can trust is an instance of the plugin interface, and recast.

