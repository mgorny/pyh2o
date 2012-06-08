/* pyh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#include "h2o.h"

static PyObject* H2O_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static int H2O_init(PyObject* _self, PyObject* args, PyObject* kwds);

static PyObject* H2O_get_p(PyObject* _self, void* data);
static PyObject* H2O_get_T(PyObject* _self, void* data);
static PyObject* H2O_get_x(PyObject* _self, void* data);
static PyObject* H2O_get_rho(PyObject* _self, void* data);

static PyObject* H2O_get_v(PyObject* _self, void* data);
static PyObject* H2O_get_u(PyObject* _self, void* data);
static PyObject* H2O_get_h(PyObject* _self, void* data);
static PyObject* H2O_get_s(PyObject* _self, void* data);
static PyObject* H2O_get_cp(PyObject* _self, void* data);
static PyObject* H2O_get_cv(PyObject* _self, void* data);
static PyObject* H2O_get_w(PyObject* _self, void* data);

static PyGetSetDef H2O_properties[] =
{
	{"p", H2O_get_p, 0, "Pressure [MPa]", 0},
	{"T", H2O_get_T, 0, "Temperature [K]", 0},
	{"x", H2O_get_x, 0, "Dryness (0..1)", 0},
	{"rho", H2O_get_rho, 0, "Density [kg/m3]", 0},

	{"v", H2O_get_v, 0, "Specific volume [m3/kg]", 0},
	{"u", H2O_get_u, 0, "Specific internal energy [kJ/kg]", 0},
	{"h", H2O_get_h, 0, "Specific enthalpy [kJ/kg]", 0},
	{"s", H2O_get_s, 0, "Specific enthropy [kJ/kgK]", 0},
	{"cp", H2O_get_cp, 0, "Specific isobaric heat capacity [kJ/kgK]", 0},
	{"cv", H2O_get_cv, 0, "Specific isochoric heat capacity [kJ/kgK]", 0},
	{"w", H2O_get_w, 0, "Speed of sound [m/s]", 0},

	{NULL}
};

PyTypeObject H2O_type =
{
	PyObject_HEAD_INIT(NULL)
	0, /* ob_size */
	"h2o.H2O", /* tp_name */
	sizeof(H2O), /* tp_basicsize */
	0, /* tp_itemsize */
	0, /* tp_dealloc */
	0, /* tp_print */
	0, /* tp_getattr */
	0, /* tp_setattr */
	0, /* tp_compare */
	0, /* tp_repr */
	0, /* tp_as_number */
	0, /* tp_as_sequence */
	0, /* tp_as_mapping */
	0, /* tp_hash  */
	0, /* tp_call */
	0, /* tp_str */
	0, /* tp_getattro */
	0, /* tp_setattro */
	0, /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	"H2O state point", /* tp_doc */
	0, /* tp_traverse */
	0, /* tp_clear */
	0, /* tp_richcompare */
	0, /* tp_weaklistoffset */
	0, /* tp_iter */
	0, /* tp_iternext */
	0, /* tp_methods */
	0, /* tp_members */
	H2O_properties, /* tp_getset */
	0, /* tp_base */
	0, /* tp_dict */
	0, /* tp_descr_get */
	0, /* tp_descr_set */
	0, /* tp_dictoffset */
	H2O_init, /* tp_init */
	0, /* tp_alloc */
	H2O_new /* tp_new */
};

static char* new_keywords[] = {
	"p", "T", "h", "s", "x", "rho"
};

static PyObject* H2O_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	PyObject* self = type->tp_alloc(type, 0);

	if (self)
	{
		H2O* typedself = (H2O*) self;

		typedself->_data.region = H2O_REGION_OUT_OF_RANGE;
	}

	return self;
}

/* the equations sometimes result in a little negative value
 * we should avoid treating it equally with undefined */
static const double unset = -100;
static const double min_set = -10;

static int is_set(double v)
{
	return (v >= min_set) ? 1 : 0;
}

static int H2O_init(PyObject* _self, PyObject* args, PyObject* kwds)
{
	H2O* self = (H2O*) _self;

	double p = unset, T = unset, h = unset, s = unset, x = unset, rho = unset;

	int arg_count, not_matched = 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|dddddd", new_keywords,
			&p, &T, &h, &s, &x, &rho))
		return -1;

	arg_count = is_set(p) + is_set(T) + is_set(h) + is_set(s)
		+ is_set(x) + is_set(rho);

	if (arg_count != 2)
	{
		PyErr_Format(PyExc_TypeError,
				"h2o.H2O() takes exactly 2 arguments (%d given)", arg_count);
		return -1;
	}

	/* find the right tuple */
	if (is_set(p))
	{
		if (is_set(T))
			self->_data = h2o_new_pT(p, T);
		else if (is_set(h))
			self->_data = h2o_new_ph(p, h);
		else if (is_set(s))
			self->_data = h2o_new_ps(p, s);
		else if (is_set(x))
			self->_data = h2o_new_px(p, x);
		else
			not_matched = 1;
	}
	else if (is_set(T))
	{
		if (is_set(x))
			self->_data = h2o_new_Tx(T, x);
		else if (is_set(rho))
			self->_data = h2o_new_rhoT(rho, T);
		else
			not_matched = 1;
	}
	else if (is_set(h) && is_set(s))
		self->_data = h2o_new_hs(h, s);
	else
		not_matched = 1;

	if (not_matched)
	{
		PyErr_SetString(PyExc_TypeError,
				"Unsupported h2o.H2O() argument combination");
		return -1;
	}

	if (!h2o_is_valid(self->_data))
	{
		PyErr_SetString(PyExc_ValueError,
				"Parameters out-of-range");
		return -1;
	}

	return 0;
}

static PyObject* H2O_get_p(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_p(self->_data));
}

static PyObject* H2O_get_T(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_T(self->_data));
}

static PyObject* H2O_get_x(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	if (self->_data.region == H2O_REGION3)
	{
		PyErr_SetString(PyExc_ValueError,
				"x (dryness) is not defined in Region 3");
		return NULL;
	}

	return Py_BuildValue("d", h2o_get_x(self->_data));
}

static PyObject* H2O_get_rho(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_rho(self->_data));
}

static PyObject* H2O_get_v(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_v(self->_data));
}

static PyObject* H2O_get_u(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_u(self->_data));
}

static PyObject* H2O_get_h(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_h(self->_data));
}

static PyObject* H2O_get_s(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_s(self->_data));
}

static PyObject* H2O_get_cp(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_cp(self->_data));
}

static PyObject* H2O_get_cv(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_cv(self->_data));
}

static PyObject* H2O_get_w(PyObject* _self, void* data)
{
	H2O* self = (H2O*) _self;

	return Py_BuildValue("d", h2o_get_w(self->_data));
}
