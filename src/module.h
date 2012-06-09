/* pyh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#pragma once

#ifndef _MODULE_H
#define _MODULE_H 1

#include <Python.h>

static const char module_name[] = "h2o";
static const char module_doc[] = "IF97 water & steam properties using libh2o";

int types_preinit();
void types_postinit(PyObject* m);

#endif /*_MODULE_H*/
