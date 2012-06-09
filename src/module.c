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

#ifdef PY3

static PyModuleDef h2o_module = {
	PyModuleDef_HEAD_INIT, /* m_base */

	module_name, /* m_name */
	module_doc, /* m_doc */
	-1, /* m_size */
	NULL /* m_methods */
};

#else

static PyMethodDef global_methods[] = {
	{NULL}
};

#endif

#ifdef PY3
PyObject* PyInit_h2o()
#else
void inith2o()
#endif
{
	PyObject* m;

	if (!types_preinit())
#ifdef PY3
		return NULL;
#else
		return;
#endif

#ifdef PY3
	m = PyModule_Create(&h2o_module);

	if (!m)
		return NULL;
#else
	m = Py_InitModule3(module_name, global_methods, module_doc);
#endif

	types_postinit(m);

#ifdef PY3
	return m;
#endif
}
