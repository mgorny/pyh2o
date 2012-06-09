/* pyh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#include "module.h"
#include "h2o.h"

#if PY_MAJOR_VERSION >= 3
#	define PY3
#endif

int types_preinit()
{
	return PyType_Ready(&H2O_type) >= 0;
}

void types_postinit(PyObject* m)
{
	Py_INCREF(&H2O_type);
	PyModule_AddObject(m, "H2O", (PyObject*) &H2O_type);
}
