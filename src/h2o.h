/* pyh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#pragma once

#ifndef _H2O_H
#define _H2O_H 1

#include <Python.h>

#include <h2o/h2o.h>

typedef struct
{
	PyObject_HEAD

	h2o_t _data;
} H2O;

PyTypeObject H2O_type;

#endif /*_H2O_H*/
