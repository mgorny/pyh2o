/* pyh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#include <Python.h>

#if PY_MAJOR_VERSION >= 3
#	define PY3
#endif

#include "h2o.h"

#ifdef PY3

static PyModuleDef h2o_module = {
	PyModuleDef_HEAD_INIT, /* m_base */

	"h2o", /* m_name */
	"IF97 water & steam properties using libh2o", /* m_doc */
	-1, /* m_size */
	NULL /* m_methods */
};

#else

static PyMethodDef global_methods[] = {
	{NULL}
};

#endif

PyMODINIT_FUNC
inith2o()
{
	PyObject* m;

	if (PyType_Ready(&H2O_type) < 0)
		return;

#ifdef PY3
	m = PyModule_Create(&h2o_module);
#else
	m = Py_InitModule3("h2o", global_methods,
		"IF97 water & steam properties using libh2o");
#endif

	Py_INCREF(&H2O_type);
	PyModule_AddObject(m, "H2O", (PyObject*) &H2O_type);

#ifdef PY3
	return m;
#endif
}
