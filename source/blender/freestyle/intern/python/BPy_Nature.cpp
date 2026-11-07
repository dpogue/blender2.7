/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** \file source/blender/freestyle/intern/python/BPy_Nature.cpp
 *  \ingroup freestyle
 */

#include "BPy_Nature.h"

#include "BPy_Convert.h"

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////

static PyObject *BPy_Nature_and(PyObject *a, PyObject *b);
static PyObject *BPy_Nature_xor(PyObject *a, PyObject *b);
static PyObject *BPy_Nature_or(PyObject *a, PyObject *b);

/*-----------------------BPy_Nature number method definitions --------------------*/

static PyNumberMethods nature_as_number = {
	0,                              /* binaryfunc nb_add */
	0,                              /* binaryfunc nb_subtract */
	0,                              /* binaryfunc nb_multiply */
	0,                              /* binaryfunc nb_remainder */
	0,                              /* binaryfunc nb_divmod */
	0,                              /* ternaryfunc nb_power */
	0,                              /* unaryfunc nb_negative */
	0,                              /* unaryfunc nb_positive */
	0,                              /* unaryfunc nb_absolute */
	0,                              /* inquiry nb_bool */
	0,                              /* unaryfunc nb_invert */
	0,                              /* binaryfunc nb_lshift */
	0,                              /* binaryfunc nb_rshift */
	(binaryfunc)BPy_Nature_and,     /* binaryfunc nb_and */
	(binaryfunc)BPy_Nature_xor,     /* binaryfunc nb_xor */
	(binaryfunc)BPy_Nature_or,      /* binaryfunc nb_or */
	0,                              /* unaryfunc nb_int */
	0,                              /* void *nb_reserved */
	0,                              /* unaryfunc nb_float */
	0,                              /* binaryfunc nb_inplace_add */
	0,                              /* binaryfunc nb_inplace_subtract */
	0,                              /* binaryfunc nb_inplace_multiply */
	0,                              /* binaryfunc nb_inplace_remainder */
	0,                              /* ternaryfunc nb_inplace_power */
	0,                              /* binaryfunc nb_inplace_lshift */
	0,                              /* binaryfunc nb_inplace_rshift */
	0,                              /* binaryfunc nb_inplace_and */
	0,                              /* binaryfunc nb_inplace_xor */
	0,                              /* binaryfunc nb_inplace_or */
	0,                              /* binaryfunc nb_floor_divide */
	0,                              /* binaryfunc nb_true_divide */
	0,                              /* binaryfunc nb_inplace_floor_divide */
	0,                              /* binaryfunc nb_inplace_true_divide */
	0,                              /* unaryfunc nb_index */
};

/*-----------------------BPy_Nature docstring ------------------------------------*/

PyDoc_STRVAR(Nature_doc,
"Class hierarchy: int > :class:`Nature`\n"
"\n"
"Different possible natures of 0D and 1D elements of the ViewMap.\n"
"\n"
"Vertex natures:\n"
"\n"
"* Nature.POINT: True for any 0D element.\n"
"* Nature.S_VERTEX: True for SVertex.\n"
"* Nature.VIEW_VERTEX: True for ViewVertex.\n"
"* Nature.NON_T_VERTEX: True for NonTVertex.\n"
"* Nature.T_VERTEX: True for TVertex.\n"
"* Nature.CUSP: True for CUSP.\n"
"\n"
"Edge natures:\n"
"\n"
"* Nature.NO_FEATURE: True for non feature edges (always false for 1D\n"
"  elements of the ViewMap).\n"
"* Nature.SILHOUETTE: True for silhouettes.\n"
"* Nature.BORDER: True for borders.\n"
"* Nature.CREASE: True for creases.\n"
"* Nature.RIDGE: True for ridges.\n"
"* Nature.VALLEY: True for valleys.\n"
"* Nature.SUGGESTIVE_CONTOUR: True for suggestive contours.\n"
"* Nature.MATERIAL_BOUNDARY: True for edges at material boundaries.\n"
"* Nature.EDGE_MARK: True for edges having user-defined edge marks.");

/*-----------------------BPy_Nature type definition ------------------------------*/

PyTypeObject Nature_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"Nature",                       /* tp_name */
	sizeof(PyLongObject),           /* tp_basicsize */
	0,                              /* tp_itemsize */
	0,                              /* tp_dealloc */
	0,                              /* tp_print */
	0,                              /* tp_getattr */
	0,                              /* tp_setattr */
	0,                              /* tp_reserved */
	0,                              /* tp_repr */
	&nature_as_number,              /* tp_as_number */
	0,                              /* tp_as_sequence */
	0,                              /* tp_as_mapping */
	0,                              /* tp_hash  */
	0,                              /* tp_call */
	0,                              /* tp_str */
	0,                              /* tp_getattro */
	0,                              /* tp_setattro */
	0,                              /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,             /* tp_flags */
	Nature_doc,                     /* tp_doc */
	0,                              /* tp_traverse */
	0,                              /* tp_clear */
	0,                              /* tp_richcompare */
	0,                              /* tp_weaklistoffset */
	0,                              /* tp_iter */
	0,                              /* tp_iternext */
	0,                              /* tp_methods */
	0,                              /* tp_members */
	0,                              /* tp_getset */
	&PyLong_Type,                   /* tp_base */
	0,                              /* tp_dict */
	0,                              /* tp_descr_get */
	0,                              /* tp_descr_set */
	0,                              /* tp_dictoffset */
	0,                              /* tp_init */
	0,                              /* tp_alloc */
	0,                              /* tp_new */
};

/*-----------------------BPy_Nature instance definitions ----------------------------------*/

//-------------------MODULE INITIALIZATION--------------------------------
int Nature_Init(PyObject *module)
{
	if (module == NULL)
		return -1;

	if (PyType_Ready(&Nature_Type) < 0)
		return -1;
	Py_INCREF(&Nature_Type);
	PyModule_AddObject(module, "Nature", (PyObject *)&Nature_Type);

#define ADD_TYPE_CONST(id) \
	PyLong_subtype_add_to_dict(Nature_Type.tp_dict, &Nature_Type, STRINGIFY(id), Nature::id)

	// VertexNature
	ADD_TYPE_CONST(POINT);
	ADD_TYPE_CONST(S_VERTEX);
	ADD_TYPE_CONST(VIEW_VERTEX);
	ADD_TYPE_CONST(NON_T_VERTEX);
	ADD_TYPE_CONST(T_VERTEX);
	ADD_TYPE_CONST(CUSP);

	// EdgeNature
	ADD_TYPE_CONST(NO_FEATURE);
	ADD_TYPE_CONST(SILHOUETTE);
	ADD_TYPE_CONST(BORDER);
	ADD_TYPE_CONST(CREASE);
	ADD_TYPE_CONST(RIDGE);
	ADD_TYPE_CONST(VALLEY);
	ADD_TYPE_CONST(SUGGESTIVE_CONTOUR);
	ADD_TYPE_CONST(MATERIAL_BOUNDARY);
	ADD_TYPE_CONST(EDGE_MARK);

#undef ADD_TYPE_CONST

	return 0;
}

static PyObject *BPy_Nature_bitwise(PyObject *a, int op, PyObject *b)
{
	BPy_Nature *result;
	long op1, op2, v;

	if (!BPy_Nature_Check(a) || !BPy_Nature_Check(b)) {
		PyErr_SetString(PyExc_TypeError, "operands must be a Nature object");
		return NULL;
	}

	if ((op1 = PyLong_AsLong(a)) == -1 && PyErr_Occurred()) {
		PyErr_SetString(PyExc_ValueError, "operand 1: unexpected Nature value");
		return NULL;
	}
	if ((op2 = PyLong_AsLong(b)) == -1 && PyErr_Occurred()) {
		PyErr_SetString(PyExc_ValueError, "operand 2: unexpected Nature value");
		return NULL;
	}
	switch (op) {
		case '&':
			v = op1 & op2;
			break;
		case '^':
			v = op1 ^ op2;
			break;
		case '|':
			v = op1 | op2;
			break;
		default:
			PyErr_BadArgument();
			return NULL;
	}
	if (v == 0)
		result = PyObject_NewVar(BPy_Nature, &Nature_Type, 0);
	else {
		result = (BPy_Nature *)PyLong_subtype_new(&Nature_Type, v);
	}
	return (PyObject *)result;
}

static PyObject *BPy_Nature_and(PyObject *a, PyObject *b)
{
	return BPy_Nature_bitwise(a, '&', b);
}

static PyObject *BPy_Nature_xor(PyObject *a, PyObject *b)
{
	return BPy_Nature_bitwise(a, '^', b);
}

static PyObject *BPy_Nature_or(PyObject *a, PyObject *b)
{
	return BPy_Nature_bitwise(a, '|', b);
}

///////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif
