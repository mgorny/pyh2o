/* pyh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#include "module.h"

#if PY_MAJOR_VERSION < 3

static PyMethodDef global_methods[] = {
	{NULL}
};

void inith2o()
{
	PyObject* m;

	if (!types_preinit())
		return;

	m = Py_InitModule3(module_name, global_methods, module_doc);

	types_postinit(m);
}

#endif
