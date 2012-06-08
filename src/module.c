/* pyh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#include <Python.h>

#include "h2o.h"

static PyMethodDef global_methods[] = {
	{NULL}
};

#ifndef PyMODINIT_FUNC /* declarations for DLL import/export */
#	define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC
inith2o()
{
	PyObject* m;

	if (PyType_Ready(&H2O_type) < 0)
		return;

	m = Py_InitModule3("h2o", global_methods,
		"IF97 water & steam properties using libh2o");

	Py_INCREF(&H2O_type);
	PyModule_AddObject(m, "H2O", (PyObject*) &H2O_type);
}
