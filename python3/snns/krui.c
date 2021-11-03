/*

    Python interface for the SNNS kernel.
    Written by Patrick Kursawe <patrick.kursawe@web.de> 2004

    SNNS has a seperate license, see
    http://www-ra.informatik.uni-tuebingen.de/SNNS/complete_license.html

    This interface is distributed under the terms of the
    GNU lesser general public license v 2.1, see
    http://www.fsf.org/licenses/lgpl.txt
    
*/


#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <glob_typ.h>
#include <krui_typ.h>
#include <kr_ui.h>
#include <kr_const.h>
#include <ext_typ.h>
#include <kr_typ.h>
#include <kernel.h>

struct Unit;

extern FlintType (*kr_PythonOutFunctionHook)(PyObject *func, FlintType activation);
extern FlintType (*kr_PythonActFunctionHook)(PyObject *func, struct Unit *unit_ptr);

extern PyObject *(*kr_findPythonFunctionHook)(char *name, int type);

extern int (*kr_getNoOfPythonFunctionsHook)();
extern krui_err (*kr_getPythonFuncInfoHook)(int mode, struct FuncInfoDescriptor *descr);

static PyObject *make_exception(krui_err err)
{
	if(err > 0) return PyLong_FromLong(err);
	PyErr_SetString(PyExc_RuntimeError, krui_error(err));
	return NULL;
}

typedef krui_err (*int_arg_with_err_func)(int);

static PyObject *
snns_int_arg_with_err(PyObject *arg, int_arg_with_err_func func)
{
	long num;
	krui_err err;
	num = PyLong_AsLong(arg);
	if(PyErr_Occurred()) return NULL;
	err = func(num);
	if(err) {
		return make_exception(err);
	}	
	return Py_BuildValue("");
}

typedef krui_err (*string_arg_with_err_func)(char *);

static PyObject *
snns_string_arg_with_err(PyObject *arg, string_arg_with_err_func func)
{
	char *text=NULL;
	krui_err err;
	text = (char *) PyUnicode_AsUTF8(arg);
	if(text) {
		if((err=func(text))) {
			return make_exception(err);
		}
	} else return NULL;
	return Py_BuildValue("");
}

static PyObject *
wrap_pattern_set_info(pattern_set_info *ps, char *attr)
{
	PyObject *ret = NULL;
	int i, failures=0;

	if(!strcmp(attr,"number_of_pattern")) {
		ret = PyLong_FromLong(ps->number_of_pattern);
	} else if(!strcmp(attr,"virtual_no_of_pattern")) {
		ret = PyLong_FromLong(ps->virtual_no_of_pattern);
	} else if(!strcmp(attr,"output_present")) {
		ret = PyLong_FromLong(ps->output_present);
	} else if(!strcmp(attr,"fixed_fixsizes")) {
		ret = PyLong_FromLong(ps->fixed_fixsizes);
	} else if(!strcmp(attr,"in_fixsize")) {
		ret = PyLong_FromLong(ps->in_fixsize);
	} else if(!strcmp(attr,"out_fixsize")) {
		ret = PyLong_FromLong(ps->out_fixsize);
	} else if(!strcmp(attr,"in_max_dim_sizes")) {
		if((ret = PyTuple_New(ps->in_number_of_dims)))
		for(i=0; i < ps->in_number_of_dims; ++i) {
			failures += PyTuple_SetItem(ret, i,
					PyLong_FromLong(ps->in_max_dim_sizes[i]));
		}
	} else if(!strcmp(attr,"out_max_dim_sizes")) {
		if((ret = PyTuple_New(ps->out_number_of_dims)))
		for(i=0; i < ps->out_number_of_dims; ++i) {
			failures += PyTuple_SetItem(ret, i,
					PyLong_FromLong(ps->out_max_dim_sizes[i]));
		}
	} else if(!strcmp(attr,"in_min_dim_sizes")) {
		if((ret = PyTuple_New(ps->in_number_of_dims)))
		for(i=0; i < ps->in_number_of_dims; ++i) {
			failures += PyTuple_SetItem(ret, i,
					PyLong_FromLong(ps->in_min_dim_sizes[i]));
		}
	} else if(!strcmp(attr,"out_min_dim_sizes")) {
		if((ret = PyTuple_New(ps->out_number_of_dims)))
		for(i=0; i < ps->out_number_of_dims; ++i) {
			failures += PyTuple_SetItem(ret, i,
					PyLong_FromLong(ps->out_min_dim_sizes[i]));
		}
	} else if(!strcmp(attr,"class_names")) {
		if((ret = PyTuple_New(ps->classes)))
		for(i=0; i < ps->classes; ++i) {
			failures += PyTuple_SetItem(ret, i,
				PyBytes_FromString(ps->class_names[i]));
		}		
	} else if(!strcmp(attr,"class_redistribution")) {
		if((ret = PyTuple_New(ps->classes)))
		for(i=0; i < ps->classes; ++i) {
			failures += PyTuple_SetItem(ret, i,
				PyLong_FromLong(ps->class_redistribution[i]));
		}		
	} else if(!strcmp(attr,"remap_params")) {
		if((ret = PyTuple_New(ps->no_of_remap_params)))
		for(i=0; i < ps->no_of_remap_params; ++i) {
			failures += PyTuple_SetItem(ret, i,
				PyFloat_FromDouble(ps->remap_params[i]));
		}		
	} else if(!strcmp(attr,"class_distrib_active")) {
		return PyLong_FromLong(ps->class_distrib_active);
	} else if(!strcmp(attr,"remap_function")) {
		return PyBytes_FromString(ps->remap_function ? ps->remap_function : "" );
	} else {
		PyErr_Format(PyExc_AttributeError,"don't have a %s attribute",attr);	
	}	
	
	if(failures) return NULL; else	return ret;	
}

static PyObject *
wrap_pattern_descriptor(pattern_descriptor *pd, char *attr)
{
	PyObject *ret=NULL;
	int i, failures=0;
	if(!strcmp(attr,"input_dim_sizes")) {
		if((ret = PyTuple_New(pd->input_dim)))
		for(i=0; i < pd->input_dim; ++i) {
			failures += PyTuple_SetItem(ret, i,
					PyLong_FromLong(pd->input_dim_sizes[i]));
		}
	} else if(!strcmp(attr,"input_fixsize")) {
		ret = PyLong_FromLong(pd->input_fixsize);
	} else if(!strcmp(attr,"output_dim_sizes")) {
		if((ret = PyTuple_New(pd->output_dim)))
		for(i=0; i < pd->output_dim; ++i) {
			failures += PyTuple_SetItem(ret, i,
					PyLong_FromLong(pd->output_dim_sizes[i]));
		}
	} else if(!strcmp(attr,"output_fixsize")) {
		ret = PyLong_FromLong(pd->output_fixsize);
	} else if(!strcmp(attr,"my_class")) {
		ret = PyLong_FromLong(pd->my_class);
	} else {
		PyErr_Format(PyExc_AttributeError, "don't have a %s attribute",attr);
	}
	
	if(failures) return NULL; else return ret;
}


typedef struct { 
    PyObject_HEAD
    pattern_set_info psi;	    
} snns_pattern_set_info_object;

typedef struct { 
    PyObject_HEAD
    pattern_descriptor pd;	    
} snns_pattern_descriptor_object;


static PyObject *
pattern_set_info_getattr(PyObject *o, PyObject *attrname)
{
	return wrap_pattern_set_info(&((snns_pattern_set_info_object *)o)->psi, (char *) PyUnicode_AsUTF8(attrname));	
}

static PyObject *
pattern_descriptor_getattr(PyObject *o, PyObject *attrname)
{
	return wrap_pattern_descriptor(&((snns_pattern_descriptor_object *)o)->pd,( char *) PyUnicode_AsUTF8(attrname));	
}

static PyTypeObject snns_pattern_set_info_type = { 
    PyVarObject_HEAD_INIT(NULL, 0)
                             /*ob_size*/
    "krui.pattern_set_info",   /*tp_name*/
    sizeof(snns_pattern_set_info_object), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    pattern_set_info_getattr,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "equivalent of the pattern_set_info struct",           /* tp_doc */
};

static PyTypeObject snns_pattern_descriptor_type = {
    PyVarObject_HEAD_INIT(NULL , 0)
                             /*ob_size*/
    "krui.pattern_descriptor",   /*tp_name*/
    sizeof(snns_pattern_descriptor_object), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    pattern_descriptor_getattr,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "equivalent of the pattern_descriptor struct",           /* tp_doc */
};

static PyObject *
snns_error(PyObject *self, PyObject *arg)
{
	int num;
	num = (int)PyLong_AsLong(arg);
	if(PyErr_Occurred()) return NULL;
	return PyBytes_FromString(krui_error(num));
}

static PyObject *
snns_getVersion(PyObject *self, PyObject *arg)
{
	return Py_BuildValue("s",krui_getVersion());
}

static PyObject *
snns_saveNet(PyObject *self, PyObject *args)
{
	char *netname;
	char *filename;
	krui_err err;
	if(PyArg_ParseTuple(args,"sz",&filename,&netname) &&
	   (err=krui_saveNet(filename,netname))) {
			return make_exception(err);
	}

	if(PyErr_Occurred()) return NULL;
	return Py_BuildValue("");
}

static PyObject *
snns_loadNet(PyObject *self, PyObject *name)
{
	char *netname=NULL;
	char *filename;
	krui_err err;
	if((filename = (char *) PyUnicode_AsUTF8(name))) {
		if((err=krui_loadNet(filename,&netname))) {
			return make_exception(err);
		}
	}
	if(PyErr_Occurred()) return NULL;
	return Py_BuildValue("s",netname);
}

static PyObject *
snns_getNetInfo(PyObject *self, PyObject *arg)
{
	int nsites, nlinks,nstent, nftent;
	krui_getNetInfo(&nsites,&nlinks,&nstent,&nftent);	
	return Py_BuildValue("(iiii)",nsites, nlinks,nstent,nftent);
}

static PyObject *
snns_getMemoryManagerInfo(PyObject *self, PyObject *arg)
{
	int ubytes, sbytes,lbytes,ntbytes,stbytes,ftbytes;
	krui_getMemoryManagerInfo(&ubytes,&sbytes,&lbytes,&ntbytes,&stbytes,&ftbytes);	
	return Py_BuildValue("(iiiii)",ubytes,sbytes,lbytes,ntbytes,
		stbytes,ftbytes);
}

static PyObject *
snns_getSymbolTable(PyObject *self, PyObject *arg)
{
	PyObject *ret;
	char *name;
	int val;
	ret = PyDict_New();
	if(!ret) return NULL;
	if(krui_getFirstSymbolTableEntry(&name,&val))
	do {
		PyDict_SetItemString(ret,name,PyLong_FromLong(val));
	} while(krui_getNextSymbolTableEntry(&name,&val));
	if(PyErr_Occurred()) return NULL;
	return ret;
}

static PyObject *
snns_deleteNet(PyObject *self, PyObject *arg)
{
	krui_deleteNet();
	return Py_BuildValue("");
}

static PyObject *
snns_setSeedNo(PyObject *self, PyObject *arg)
{
	long num;
	num = PyLong_AsLong(arg);
	if(PyErr_Occurred()) return NULL;
	krui_setSeedNo(num);
	return Py_BuildValue("");
}

static PyObject *
snns_setPatternNo(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg,krui_setPatternNo);
}

static PyObject *
snns_setCurrPatSet(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg,krui_setCurrPatSet);
}

static PyObject *
snns_deletePattern(PyObject *self, PyObject *arg)
{
	krui_err err;
	if((err = krui_deletePattern())) {
		return make_exception(err);
	}
	return Py_BuildValue("");
}

static PyObject *
snns_deleteAllPatterns(PyObject *self, PyObject *arg)
{
	/* krui_err err;   dan     */ 
	krui_deleteAllPatterns();
	return Py_BuildValue("");
}

static PyObject *
snns_shufflePatterns(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg,krui_shufflePatterns);
}

static PyObject *
snns_shuffleSubPatterns(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg,krui_shuffleSubPatterns);
}

static PyObject *
snns_modifyPattern(PyObject *self, PyObject *arg)
{
	krui_err err;
	if((err = krui_modifyPattern())) {
		return make_exception(err);
	}
	return Py_BuildValue("");
}

static PyObject *
snns_allocateUnits(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg,krui_allocateUnits);
}

static PyObject *
snns_getNoOfOutputUnits(PyObject *self, PyObject *arg)
{
	return Py_BuildValue("i",krui_getNoOfOutputUnits());
}

static PyObject *
snns_getNoOfInputUnits(PyObject *self, PyObject *arg)
{
	return Py_BuildValue("i",krui_getNoOfInputUnits());
}

static PyObject *
snns_getPatternNo(PyObject *self, PyObject *arg)
{
	return Py_BuildValue("i",krui_getPatternNo());
}

static PyObject *
snns_getNoOfPatterns(PyObject *self, PyObject *arg)
{
	return Py_BuildValue("i",krui_getNoOfPatterns());
}

static PyObject *
snns_getTotalNoOfSubPatterns(PyObject *self, PyObject *arg)
{
	return Py_BuildValue("i",krui_getTotalNoOfSubPatterns());
}

static PyObject *
snns_newPattern(PyObject *self, PyObject *arg)
{
	krui_err err;
	if((err = krui_newPattern())) {
		return make_exception(err);
	}
	return Py_BuildValue("");
}

static PyObject *
snns_allocNewPatternSet(PyObject *self, PyObject *arg)
{
	krui_err err;
	int i;
	if((err = krui_allocNewPatternSet(&i))) {
		return make_exception(err);
	}
	return PyLong_FromLong(i);
}

static PyObject *
snns_resetNet(PyObject *self, PyObject *arg)
{
	krui_resetNet();
	return Py_BuildValue("");
}

static PyObject *
snns_getNoOfTTypeUnits(PyObject *self, PyObject *arg)
{
	int no;
	int parm;
	parm = PyLong_AsLong(arg);
	if(PyErr_Occurred()) return NULL;
	no = krui_getNoOfTTypeUnits(parm);
	return PyLong_FromLong(no);
}

static PyObject *
snns_getLearnFunc(PyObject *self, PyObject *arg)
{
	return PyUnicode_FromString(krui_getLearnFunc());
}

static PyObject *
snns_getFFLearnFunc(PyObject *self, PyObject *arg)
{
	return PyUnicode_FromString(krui_getFFLearnFunc());
}

static PyObject *
snns_getPrunFunc(PyObject *self, PyObject *arg)
{
	return PyUnicode_FromString(krui_getPrunFunc());
}


static PyObject *
snns_setLearnFunc(PyObject *self, PyObject *name)
{
	return snns_string_arg_with_err(name,krui_setLearnFunc);
}

static PyObject *
snns_getInitialisationFunc(PyObject *self, PyObject *arg)
{
	return PyUnicode_FromString(krui_getInitialisationFunc());
}

static PyObject *
snns_setInitialisationFunc(PyObject *self, PyObject *name)
{
	return snns_string_arg_with_err(name,krui_setInitialisationFunc);
}

static PyObject *
snns_getUpdateFunc(PyObject *self, PyObject *arg)
{
	return PyUnicode_FromString(krui_getUpdateFunc());
}

static PyObject *
snns_setUpdateFunc(PyObject *self, PyObject *name)
{
	return snns_string_arg_with_err(name,krui_setUpdateFunc);
}

static PyObject *
snns_setFFLearnFunc(PyObject *self, PyObject *name)
{
	return snns_string_arg_with_err(name,krui_setFFLearnFunc);
}

static PyObject *
snns_setPrunFunc(PyObject *self, PyObject *name)
{
	return snns_string_arg_with_err(name,krui_setPrunFunc);
}

static PyObject *
snns_loadNewPatterns(PyObject *self, PyObject *arg)
{
	char *fn=NULL;
	krui_err err;
	int setno;

	fn =  (char *)  PyUnicode_AsUTF8(arg);
	if(fn && (err=krui_loadNewPatterns(fn,&setno)))
       	{
		return make_exception(err);
	}
	if(PyErr_Occurred())
	       	return NULL;
	return Py_BuildValue("i",setno);
}

static PyObject *
snns_saveNewPatterns(PyObject *self, PyObject *args)
{
	int setnum;
	char *filename=NULL;
	krui_err err;
	if(PyArg_ParseTuple(args,"si",&filename,&setnum) &&
	   (err=krui_saveNewPatterns(filename,setnum))) {
			return make_exception(err);
	}

	if(PyErr_Occurred()) return NULL;
	return Py_BuildValue("");
}

static PyObject *
snns_saveResultParam(PyObject *self, PyObject *args)
{
	int create,startpattern,endpattern,includeinput,includeoutput;
	char *filename;
	PyObject *updateparams, *current;
	float fparams[10];
	int nparams=0;
	int index;
	krui_err err;
	if(PyArg_ParseTuple(args,"siiiiiO",&filename,&create, &startpattern,
		&endpattern, &includeinput, &includeoutput, &updateparams)) {
		if(PySequence_Check(updateparams)) {
			nparams = PySequence_Size(updateparams);
			if(nparams > 10) {
				PyErr_SetString(PyExc_RuntimeError, "too many update parameters for wrapper");
				return NULL;
			} else {
				for(index =0; index < nparams; ++index) {
					if((current = PySequence_GetItem(updateparams,index))) {
						fparams[index]=PyFloat_AsDouble(current);
					} else {
						PyErr_SetString(PyExc_RuntimeError,"could not get item from tuple");
						return NULL;
					}
				}
				if((err=krui_saveResultParam(filename,create,startpattern,endpattern,includeinput,
					includeoutput,fparams,nparams))) {
					return make_exception(err);
				}
			}
		} else {
			PyErr_SetString(PyExc_RuntimeError, "expected tuple for update parameters");
		}
	}
	if(PyErr_Occurred()) return NULL;
	return Py_BuildValue("");
}

static PyObject *
snns_GetPatInfo(PyObject *self, PyObject *args)
{
	PyObject *patseto, *patdeso;
	krui_err err;

	Py_INCREF((PyObject *)&snns_pattern_set_info_type);
	patseto = PyType_GenericNew(&snns_pattern_set_info_type, NULL,NULL);
	if(!patseto) return NULL;

	patdeso = PyType_GenericNew(&snns_pattern_descriptor_type, NULL, NULL);
	if(!patdeso) return NULL;
	
	if((err = krui_GetPatInfo(&((snns_pattern_set_info_object *)patseto)->psi,
				&((snns_pattern_descriptor_object *)patdeso)->pd))) {
		return make_exception(err);
	}
	return Py_BuildValue("(OO)",patseto, patdeso);
				
}

/*
	Build a tuple of integers from an integer array
*/

static PyObject *
build_int_tuple(int *array, size_t size)
{
	PyObject *ret;
	int i;
	ret = PyTuple_New(size);
	if(!ret) return NULL;
	for(i=0; i < size; ++i) {
		PyTuple_SetItem(ret,i,PyLong_FromLong(array[i]));
	}
	if(PyErr_Occurred()) return NULL;
	return ret;
}

/*
	Fill an integer array from a tuple
	The array must be large enough, this function
	does not check the size
*/

bool fill_int_array(PyObject *seq, int *array)
{
	int i;
	for(i=0; i < PySequence_Size(seq); ++i) {
		array[i]=PyLong_AsLong(PySequence_GetItem(seq,i));	
	}
	if(PyErr_Occurred()) return FALSE;
	return TRUE;
}

static PyObject *
snns_GetShapeOfSubPattern(PyObject *self, PyObject *args)
{
	int num;
	int insize[MAX_NO_OF_VAR_I_DIM],
		inpos[MAX_NO_OF_VAR_I_DIM],
		outsize[MAX_NO_OF_VAR_O_DIM],
		outpos[MAX_NO_OF_VAR_O_DIM];
	krui_err err;
	pattern_set_info setinf;
	pattern_descriptor patdes;
	PyObject *insizet, *inpost, *outsizet, *outpost;

	num = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	
	err = krui_GetPatInfo(&setinf, &patdes);
	if(err) return make_exception(err);
	err = krui_GetShapeOfSubPattern(insize,outsize,inpos,outpos,num);
	if(err) return make_exception(err);
	insizet = build_int_tuple(insize,patdes.input_dim);
	inpost = build_int_tuple(inpos,patdes.input_dim);
	outsizet = build_int_tuple(outsize,patdes.output_dim);
	outpost = build_int_tuple(outpos,patdes.output_dim);
	if(PyErr_Occurred()) return NULL;
	return Py_BuildValue("(OOOO)",insizet,outsizet,inpost,outpost);
}

static PyObject *
snns_setClassDistribution(PyObject *self, PyObject *args)
{
	/* int num; */
	PyObject *ret=NULL, *tmp=NULL;
	int *entries=NULL;
	pattern_set_info setinf;
	pattern_descriptor patdes;
	krui_err err;
	if(PySequence_Size(args) > 0) {
		tmp = PySequence_GetItem(args,0);	
		if(PySequence_Check(tmp)) {
			if(PySequence_Size(args) > 1) {
				PyErr_SetString(PyExc_RuntimeError,
				 "sorry, no nested types here!");
				 return NULL;
			}
			args = tmp;
		}
	}	
	err = krui_GetPatInfo(&setinf, &patdes);
	if(err) return make_exception(err);
	entries = PyMem_New(int,setinf.classes);
	if(!entries) {
		return PyErr_NoMemory();
	}
	if(fill_int_array(args,entries)) {
		err = krui_setClassDistribution( (unsigned int * )  entries);
		if(err) {
			ret = make_exception(err);
		} else {
			ret = Py_BuildValue("");
		}	
	}
	PyMem_Del(entries);
	return ret;
}

/*
	builds the necessary arrays for Def*SubPat from
	sequence arguments
*/

static bool prep_subpat_arrays(PyObject *args,
				int *insize, int *outsize,
				int *inparm, int *outparm)
{
	PyObject *insizet=NULL, *inparmt=NULL,
		 *outsizet=NULL, *outparmt=NULL;
	int i;

	if(!PyArg_ParseTuple(args,"|OOOO",&insizet, &outsizet, &inparmt, &outparmt)) {
		return FALSE;
	}
	if(!insizet) insizet=PyTuple_New(0);
	if(!inparmt) inparmt=PyTuple_New(0);
	if(!outsizet) outsizet=PyTuple_New(0);
	if(!outparmt) outparmt=PyTuple_New(0);
	
	if(PySequence_Size(insizet) > MAX_NO_OF_VAR_I_DIM ||
	   PySequence_Size(inparmt) > MAX_NO_OF_VAR_I_DIM ||
	   PySequence_Size(outsizet) > MAX_NO_OF_VAR_O_DIM ||
	   PySequence_Size(outparmt) > MAX_NO_OF_VAR_O_DIM) {
		PyErr_SetString(PyExc_RuntimeError, "too many dimensions");
		return FALSE;
	}
	if(PySequence_Size(insizet) != PySequence_Size(inparmt) ||
	   PySequence_Size(outsizet) != PySequence_Size(outparmt)) {
	   	PyErr_SetString(PyExc_RuntimeError, "dimension mismatch");
		return FALSE;
	}
	for(i=0; i < PySequence_Size(insizet); ++i) {
		insize[i] = PyLong_AsLong(PySequence_GetItem(insizet,i));
		inparm[i] = PyLong_AsLong(PySequence_GetItem(inparmt,i));
	}
	for(i=0; i < PySequence_Size(outsizet); ++i) {
		outsize[i] = PyLong_AsLong(PySequence_GetItem(outsizet,i));
		outparm[i] = PyLong_AsLong(PySequence_GetItem(outparmt,i));
	}
	if(PyErr_Occurred()) return FALSE;
	return TRUE;
}

static PyObject *
snns_DefTrainSubPat(PyObject *self, PyObject *args)
{
	int insize[MAX_NO_OF_VAR_I_DIM],
		instep[MAX_NO_OF_VAR_I_DIM],
		outsize[MAX_NO_OF_VAR_O_DIM],
		outstep[MAX_NO_OF_VAR_O_DIM];
	krui_err err;
	/* int i, */
	int max_n_pos;

	memset(insize,0,sizeof(insize));
	memset(instep,0,sizeof(instep));
	memset(outsize,0,sizeof(outsize));
	memset(outstep,0,sizeof(outstep));

	if(!prep_subpat_arrays(args,insize,outsize,instep,outstep)) return NULL;
	
	if((err = krui_DefTrainSubPat(insize,outsize,instep,outstep,&max_n_pos))) {
		return make_exception(err);
	}
	return PyLong_FromLong(max_n_pos);
}

static PyObject *
snns_DefShowSubPat(PyObject *self, PyObject *args)
{
	int insize[MAX_NO_OF_VAR_I_DIM],
		inpos[MAX_NO_OF_VAR_I_DIM],
		outsize[MAX_NO_OF_VAR_O_DIM],
		outpos[MAX_NO_OF_VAR_O_DIM];
	krui_err err;
	/* int i; */
	memset(insize,0,sizeof(insize));
	memset(inpos,0,sizeof(inpos));
	memset(outsize,0,sizeof(outsize));
	memset(outpos,0,sizeof(outpos));

	
	if(!prep_subpat_arrays(args,insize,outsize,inpos,outpos)) return NULL;
	
	if((err = krui_DefShowSubPat(insize,outsize,inpos,outpos))) {
		return make_exception(err);
	}
	return Py_BuildValue("");
}

static PyObject *
snns_AlignSubPat(PyObject *self, PyObject *args)
{
	int inpos[MAX_NO_OF_VAR_I_DIM],
		outpos[MAX_NO_OF_VAR_O_DIM];
	krui_err err;
	int no;
	PyObject *inpost, *outpost;
	if(!PyArg_ParseTuple(args,"OO",&inpost,&outpost)) {
		return NULL;
	}
	if(PySequence_Size(inpost) > MAX_NO_OF_VAR_I_DIM ||
	   PySequence_Size(outpost) > MAX_NO_OF_VAR_O_DIM) {
		PyErr_SetString(PyExc_RuntimeError,"too many dimensions");
		return NULL;
	}   
	if(fill_int_array(inpost,inpos) && fill_int_array(outpost,outpos)) {	
		err = krui_AlignSubPat(inpos,outpos,&no);
		if(err)
		       	return make_exception(err);
		return PyLong_FromLong(no);
	}
	return NULL;
}

static PyObject *
snns_setRemapFunc(PyObject *self, PyObject *args)
{
	PyObject *params;
	char * name;
	float *fparams;
	int i, num;
	krui_err err;
	if(!PyArg_ParseTuple(args,"sO",&name,&params)) {
		return NULL;
	}
	if(!PySequence_Check(params)) {
		PyErr_SetString(PyExc_RuntimeError,"expecting a sequence as second parameter");
		return NULL;
	}	
	num = PySequence_Size(params);
	if(PyErr_Occurred()) return NULL;
	if(!(fparams = PyMem_New(float,num))) {
		
		return PyErr_NoMemory();
	}
	for(i = 0; i < num; ++i) {
		fparams[i] = PyFloat_AsDouble(PySequence_GetItem(params,i));
	}
	err = krui_setRemapFunc(name, fparams);
	PyMem_Del(fparams);
	if(err) {
		return make_exception(err);
	}
	if(PyErr_Occurred()) return NULL;
	return Py_BuildValue("");
}

static PyObject *
snns_showPattern(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg,krui_showPattern);
}

static PyObject *
snns_deletePatSet(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg,krui_deletePatSet);
}

static PyObject *
snns_useClassDistribution(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg,krui_useClassDistribution);
}

static PyObject *
snns_setClassInfo(PyObject *self, PyObject *arg)
{
	return snns_string_arg_with_err(arg,krui_setClassInfo);
}

typedef krui_err (*learn_patterns_func)(float *,int,float **,int *);
typedef krui_err (*learn_single_pattern_func)(int, float *,int,float **,int *);

static PyObject *
snns_learn_general_single_pattern(PyObject *args, learn_single_pattern_func func)
{
	float *params, *outparms;
	int patnum,i,outnum;
	PyObject *ret, *seq;
	krui_err err;
	
	if(!PyArg_ParseTuple(args,"iO",&patnum,&seq)) return NULL;
	
	if(!PySequence_Check(seq)) {
		PyErr_SetString(PyExc_RuntimeError, "expecting a sequence of learning parameters");
		return NULL;
	}
	params = PyMem_New(float,PySequence_Size(seq));
	
	if(!params) return PyErr_NoMemory();
	for(i=0; i < PySequence_Size(seq); ++i) {
		params[i]=PyFloat_AsDouble(PySequence_GetItem(seq,i));
	}
	if(PyErr_Occurred()) {
		PyMem_Del(params);
		ret = NULL;
	} else {
		err = func(patnum, params,PySequence_Size(seq),
			&outparms,&outnum);
		if(err) ret=make_exception(err);
		else {
			ret = PyTuple_New(outnum);
			if(ret) {
				for(i=0; i < outnum; ++i) {
					PyTuple_SetItem(ret,i,
					PyFloat_FromDouble(outparms[i]));
				}
			}
		}
	}	
	PyMem_Del(params);
	return ret;	
}

static PyObject *
snns_learnSinglePattern(PyObject *self, PyObject *args)
{
	return snns_learn_general_single_pattern(args, krui_learnSinglePattern);
}

static PyObject *
snns_learnSinglePatternFF(PyObject *self, PyObject *args)
{
	return snns_learn_general_single_pattern(args, krui_learnSinglePatternFF);
}

static PyObject *
snns_learn_general_patterns(PyObject *args, learn_patterns_func func)
{
	float *params, *outparms;
	int i,outnum;
	PyObject *ret, *tmp;
	krui_err err;

	if(PySequence_Size(args) > 0) {
		tmp = PySequence_GetItem(args,0);	
		if(PySequence_Check(tmp)) {
			if(PySequence_Size(args) > 1) {
				PyErr_SetString(PyExc_RuntimeError,
				 "sorry, no nested types here!");
				 return NULL;
			}
			args = tmp;
		}
	}	
	if(!PySequence_Check(args)) {
		PyErr_SetString(PyExc_RuntimeError, "expecting a sequence of learning parameters");
		return NULL;
	}
	params = PyMem_New(float,PySequence_Size(args));
	
	if(!params) return PyErr_NoMemory();
	for(i=0; i < PySequence_Size(args); ++i) {
		params[i]=PyFloat_AsDouble(PySequence_GetItem(args,i));
	}
	if(PyErr_Occurred()) {
		PyMem_Del(params);
		ret = NULL;
	} else {
		err = func(params,PySequence_Size(args),
			&outparms,&outnum);
		if(err) ret=make_exception(err);
		else {
			ret = PyTuple_New(outnum);
			if(ret) {
				for(i=0; i < outnum; ++i) {
					PyTuple_SetItem(ret,i,
					PyFloat_FromDouble(outparms[i]));
				}
			}
		}
	}	
	PyMem_Del(params);
	return ret;	
}

static PyObject *
snns_learnAllPatterns(PyObject *self, PyObject *arg)
{
	return snns_learn_general_patterns(arg,krui_learnAllPatterns);
}

static PyObject *
snns_learnAllPatternsFF(PyObject *self, PyObject *arg)
{
	return snns_learn_general_patterns(arg,krui_learnAllPatternsFF);
}

static PyObject *
snns_updateSingleUnit(PyObject *self, PyObject *arg)
{
	return snns_int_arg_with_err(arg, krui_updateSingleUnit);
}

typedef krui_err (*snns_floatarray_arg_with_err_f)(float *, int);

static PyObject *
snns_floatarray_arg_with_err(PyObject *arg, snns_floatarray_arg_with_err_f func)
{
	krui_err err;
	float *array=NULL;
	int numparm;
	unsigned i;
	PyObject *tmp;
	if(PySequence_Size(arg) > 0) {
		tmp = PySequence_GetItem(arg,0);	
		if(PySequence_Check(tmp)) {
			if(PySequence_Size(arg) > 1) {
				PyErr_SetString(PyExc_RuntimeError,
				 "sorry, no nested types here!");
				 return NULL;
			}
			arg = tmp;
		}
	}
	if(!PySequence_Check(arg)) {
		PyErr_SetString(PyExc_RuntimeError, "expecting a sequence of update parameters");
	} else {
		numparm = PySequence_Size(arg);
		array = PyMem_New(float, numparm);
		if(array) {
			for(i=0; i < numparm; ++i) {
				array[i]=PyFloat_AsDouble(PySequence_GetItem(arg,i));
			}	
			err = func(array,numparm);
			if(err) make_exception(err);
			PyMem_Del(array);
		}
	}
	if(PyErr_Occurred()) return NULL;
	else return Py_BuildValue("");
}

static PyObject *
snns_updateNet(PyObject *self, PyObject *arg)
{
	return snns_floatarray_arg_with_err(arg, krui_updateNet);
}

static PyObject *
snns_initializeNet(PyObject *self, PyObject *arg)
{
	return snns_floatarray_arg_with_err(arg, krui_initializeNet);
}

static PyObject *
snns_getNoOfFunctions(PyObject *self, PyObject *arg)
{
	return PyLong_FromLong(krui_getNoOfFunctions());
}

static PyObject *
snns_getFuncInfo(PyObject *self, PyObject *arg)
{
	int funcno,functype;
	char *funcname;
	funcno = PyLong_AsLong(arg);
	if(PyErr_Occurred()) return NULL;
	krui_getFuncInfo(funcno, &funcname, &functype);
	return Py_BuildValue("(si)",funcname,functype);
}

static PyObject *
snns_isFunction(PyObject *self, PyObject *args)
{
	char *name;
	int functype;
	if(!PyArg_ParseTuple(args, "si",&name, &functype)) return NULL;
	return PyLong_FromLong(krui_isFunction(name,functype));
}

static PyObject *
snns_getFuncParamInfo(PyObject *self, PyObject *args)
{
	char *name;
	int functype,noin,noout;
	int isfunc;
	if(!PyArg_ParseTuple(args, "si",&name, &functype)) return NULL;
	isfunc = krui_getFuncParamInfo(name,functype,&noin,&noout);
	if(isfunc) return Py_BuildValue("(ii)",noin,noout);
	else return PyTuple_New(0);
}

static PyObject *
snns_setFirstFTypeEntry(PyObject *self, PyObject *args)
{
	return PyLong_FromLong(krui_setFirstFTypeEntry());
}

static PyObject *
snns_setNextFTypeEntry(PyObject *self, PyObject *args)
{
	return PyLong_FromLong(krui_setNextFTypeEntry());
}

static PyObject *
snns_setFTypeEntry(PyObject *self, PyObject *args)
{
	char *ftypename;
	ftypename = (char *) PyUnicode_AsUTF8(args);
	if(PyErr_Occurred()) return NULL;
	return PyLong_FromLong(krui_setFTypeEntry(ftypename));
}

static PyObject *
snns_getFTypeName(PyObject *self, PyObject *args)
{
	char *ftype;
	ftype = krui_getFTypeName();
	if(ftype) return PyUnicode_FromString(krui_getFTypeName());
	else return Py_BuildValue("");
}

static PyObject *
snns_setFTypeName(PyObject *self, PyObject *args)
{
	return snns_string_arg_with_err(args,krui_setFTypeName);
}

static PyObject *
snns_getFTypeActFuncName(PyObject *self, PyObject *args)
{
	char *funcname;
	funcname = krui_getFTypeActFuncName();
	if(funcname) return PyUnicode_FromString(funcname);
	else return Py_BuildValue("");
}

static PyObject *
snns_setFTypeActFunc(PyObject *self, PyObject *args)
{
	return snns_string_arg_with_err(args,krui_setFTypeActFunc);
}

static PyObject *
snns_getFTypeOutFuncName(PyObject *self, PyObject *args)
{
	char *funcname;
	funcname = krui_getFTypeOutFuncName();
	if(funcname) return PyUnicode_FromString(funcname);
	else return Py_BuildValue("");
}

static PyObject *
snns_setFTypeOutFunc(PyObject *self, PyObject *args)
{
	return snns_string_arg_with_err(args,krui_setFTypeOutFunc);
}

static PyObject *
snns_setFirstFTypeSite(PyObject *self, PyObject *args)
{
	return PyLong_FromLong(krui_setFirstFTypeSite());
}

static PyObject *
snns_setNextFTypeSite(PyObject *self, PyObject *args)
{
	return PyLong_FromLong(krui_setNextFTypeSite());
}

static PyObject *
snns_getFTypeSiteName(PyObject *self, PyObject *args)
{
	char *funcname;
	funcname = krui_getFTypeSiteName();
	if(funcname) return PyUnicode_FromString(funcname);
	else return Py_BuildValue("");
}

static PyObject *
snns_setFTypeSiteName(PyObject *self, PyObject *args)
{
	return snns_string_arg_with_err(args,krui_setFTypeSiteName);
}

static PyObject *
snns_deleteFTypeEntry(PyObject *self, PyObject *args)
{
	char *ftypename;
	krui_err err;
	ftypename = (char *)  PyUnicode_AsUTF8(args);
	if(PyErr_Occurred()) return NULL;
	err = krui_deleteFTypeEntry(ftypename);
	if(err) return make_exception(err);
	return Py_BuildValue("");
}

static PyObject *
snns_createFTypeEntry(PyObject *self, PyObject *args)
{
	char *ftypename, *actfuncname, *outfuncname;
	PyObject *sitenames=NULL;
	char **nameslist=NULL;
	int i;
	int numsites=0;
	krui_err err;
	if(!PyArg_ParseTuple(args,"sss|O",&ftypename,&actfuncname,&outfuncname,&sitenames)) return NULL;
	if(sitenames) {
		numsites = PySequence_Size(sitenames);
		nameslist = PyMem_New(char *, numsites);
		if(!nameslist) return NULL;
		for(i=0; i < numsites; ++i) {
			nameslist[i]  =  (char *)  PyUnicode_AsUTF8(PySequence_GetItem(sitenames,i));
		}
	}
	err = krui_createFTypeEntry(ftypename,actfuncname,outfuncname,
		numsites,nameslist);
	if(nameslist) PyMem_Del(nameslist);	
	if(err) return make_exception(err);	
	return Py_BuildValue("");
}

static PyObject *
snns_getFirstPredUnit(PyObject *self, PyObject *args)
{
	FlintType strength;
	int ret;
	ret = krui_getFirstPredUnit(&strength);
	return Py_BuildValue("(if)",ret,strength);
}

static PyObject *
snns_getFirstPredUnitAndData(PyObject *self, PyObject *args)
{
	FlintType strength;
	float a,b,c;
	int ret;
	a = b = c = 0;
	ret = krui_getFirstPredUnitAndData(&strength,&a,&b,&c);
	return Py_BuildValue("(iffff)",ret,strength,a,b,c);
}


static PyObject *
snns_getNextPredUnit(PyObject *self, PyObject *args)
{
	FlintType strength;
	int ret;
	ret = krui_getNextPredUnit(&strength);
	return Py_BuildValue("(if)",ret,strength);
}

static PyObject *
snns_getNextPredUnitAndData(PyObject *self, PyObject *args)
{
	FlintType strength;
	float a,b,c;
	int ret;
	a = b = c = 0;
	ret = krui_getNextPredUnitAndData(&strength,&a,&b,&c);
	return Py_BuildValue("(iffff)",ret,strength,a,b,c);
}

static PyObject *
snns_getCurrentPredUnit(PyObject *self, PyObject *args)
{
	FlintType strength;
	int ret;
	ret = krui_getCurrentPredUnit(&strength);
	return Py_BuildValue("(if)",ret,strength);
}

static PyObject *
snns_getFirstSuccUnit(PyObject *self, PyObject *args)
{
	FlintType strength;
	int ret,unit;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_getFirstSuccUnit(unit,&strength);
	return Py_BuildValue("(if)",ret,strength);
}

static PyObject *
snns_getNextSuccUnit(PyObject *self, PyObject *args)
{
	FlintType strength;
	int ret;
	ret = krui_getNextSuccUnit(&strength);
	return Py_BuildValue("(if)",ret,strength);
}

static PyObject *
snns_isConnected(PyObject *self, PyObject *args)
{
	int ret,unit;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_isConnected(unit);
	return PyLong_FromLong(ret);
}

static PyObject *
snns_areConnected(PyObject *self, PyObject *args)
{
	FlintType strength;
	int ret,unit,unit2;
	strength = 0;
	if(!PyArg_ParseTuple(args,"ii",&unit,&unit2)) return NULL;
	ret = krui_areConnectedWeight(unit,unit2,&strength);
	if(ret) return PyFloat_FromDouble(strength);
	else return Py_BuildValue("");
}

static PyObject *
snns_getLinkWeight(PyObject *self, PyObject *args)
{
	return PyFloat_FromDouble(krui_getLinkWeight());
}

static PyObject *
snns_setLinkWeight(PyObject *self, PyObject *args)
{
	double flt;
	flt = PyFloat_AsDouble(args);
	if(PyErr_Occurred()) return NULL;
	krui_setLinkWeight(flt);
	return Py_BuildValue("");
}

static PyObject *
snns_createLink(PyObject *self, PyObject *args)
{
	double weight;
	int unit;
	krui_err err;
	if(!PyArg_ParseTuple(args,"id",&unit,&weight)) return NULL;
	err = krui_createLink(unit,weight);
	if(err) return make_exception(err);
	return Py_BuildValue("");
}

static PyObject *
snns_createLinkWithAdditionalParameters(PyObject *self, PyObject *args)
{
	double weight,v1,v2,v3;
	int unit;
	krui_err err;
	if(!PyArg_ParseTuple(args,"idddd",&unit,&weight,&v1,&v2,&v3)) return NULL;
	krui_createLinkWithAdditionalParameters(unit,weight,v1,v2,v3);
	err = krui_getErrorCode();
	if(err) return make_exception(err);
	return Py_BuildValue("");
}

static PyObject *
snns_getErrorCode(PyObject *self, PyObject *args)
{
	return PyLong_FromLong(krui_getErrorCode());
}

static PyObject *
snns_deleteLink(PyObject *self, PyObject *args)
{
	krui_err err;
	err = krui_deleteLink();
	if(err) return make_exception(err);
	return Py_BuildValue("");
}

static PyObject *
snns_deleteAllInputLinks(PyObject *self, PyObject *args)
{
	krui_err err;
	err = krui_deleteAllInputLinks();
	if(err) return make_exception(err);
	return Py_BuildValue("");
}


static PyObject *
snns_deleteAllOutputLinks(PyObject *self, PyObject *args)
{
	krui_err err;
	err = krui_deleteAllOutputLinks();
	if(err) return make_exception(err);
	return Py_BuildValue("");
}

static PyObject *
snns_jogWeights(PyObject *self, PyObject *args)
{
	double minus, plus;
	if(!PyArg_ParseTuple(args,"dd",&minus,&plus)) return NULL;
	krui_jogWeights(minus,plus);
	return Py_BuildValue("");
}

static PyObject *
snns_jogCorrWeights(PyObject *self, PyObject *args)
{
	double minus, plus, mincorr;
	krui_err err;
	if(!PyArg_ParseTuple(args,"ddd",&minus,&plus,&mincorr)) return NULL;
	err = krui_jogCorrWeights(minus,plus,mincorr);
	if(err) return make_exception(err);
	return Py_BuildValue("");
}

static PyObject *
snns_createSiteTableEntry(PyObject *self, PyObject *args)
{
	krui_err err;
	char *a, *b;
	if(!PyArg_ParseTuple(args,"ss",&a,&b)) return NULL;
	err = krui_createSiteTableEntry(a,b);
	if(err) return make_exception(err);
	return Py_BuildValue("");
}	

static PyObject *
snns_changeSiteTableEntry(PyObject *self, PyObject *args)
{
	krui_err err;
	char *a, *b, *c;
	if(!PyArg_ParseTuple(args,"sss",&a,&b,&c)) return NULL;
	err = krui_changeSiteTableEntry(a,b,c);
	if(err) return make_exception(err);
	return Py_BuildValue("");
}	

static PyObject *
snns_deleteSiteTableEntry(PyObject *self, PyObject *args)
{
	return snns_string_arg_with_err(args,krui_deleteSiteTableEntry);
}	

static PyObject *
snns_getSiteTable(PyObject *self, PyObject *args)
{	
	bool ret;
	char *a,*b;
	PyObject *dict;
	dict = PyDict_New();
	if(!dict) return NULL;
	ret = krui_getFirstSiteTableEntry(&a,&b);
	if(!ret) return Py_BuildValue("{}");
	if(PyDict_SetItemString(dict, a, PyUnicode_FromString(b)) == -1) return NULL;
	while(krui_getNextSiteTableEntry(&a,&b)) {
		if(PyDict_SetItemString(dict, a, PyUnicode_FromString(b)) == -1) return NULL;
	}
	if(PyErr_Occurred()) return NULL;
	return dict;
}	

static PyObject *
snns_getSiteTableFuncName(PyObject *self, PyObject *args)
{	
	char *a,*b;
	a = (char *) PyUnicode_AsUTF8(args);
	if(PyErr_Occurred()) return NULL;
	b = krui_getSiteTableFuncName(a);
	if(b) return PyUnicode_FromString(b);
	else return Py_BuildValue("");
}	

static PyObject *
snns_setFirstSite(PyObject *self, PyObject *args)
{	
	int ret;
	ret = krui_setFirstSite();
	if(ret < 0) return make_exception(ret);
	return PyLong_FromLong(ret);
}	

static PyObject *
snns_setNextSite(PyObject *self, PyObject *args)
{	
	int ret;
	ret = krui_setNextSite();
	if(ret < 0) return make_exception(ret);
	return PyLong_FromLong(ret);
}	

static PyObject *
snns_setSite(PyObject *self, PyObject *args)
{
	return snns_string_arg_with_err(args,krui_setSite);
}	

static PyObject *
snns_getSiteValue(PyObject *self, PyObject *args)
{
	return PyFloat_FromDouble(krui_getSiteValue());
}	

static PyObject *
snns_getSiteFuncName(PyObject *self, PyObject *args)
{
	char *ret;
	ret = krui_getSiteFuncName();
	if(ret) return PyUnicode_FromString(ret);
	else return Py_BuildValue("");
}	

static PyObject *
snns_getSiteName(PyObject *self, PyObject *args)
{
	char *ret;
	ret = krui_getSiteName();
	if(ret) return PyUnicode_FromString(ret);
	else return Py_BuildValue("");
}

static PyObject *
snns_setSiteName(PyObject *self, PyObject *args)
{
	return snns_string_arg_with_err(args,krui_setSiteName);
}	

static PyObject *
snns_addSite(PyObject *self, PyObject *args)
{
	return snns_string_arg_with_err(args,krui_addSite);
}	

static PyObject *
snns_deleteSite(PyObject *self, PyObject *args)
{	
	return PyLong_FromLong(krui_deleteSite());
}	

static PyObject *
snns_getNoOfUnits(PyObject *self, PyObject *args)
{	
	return PyLong_FromLong(krui_getNoOfUnits());
}	

static PyObject *
snns_getNoOfSpecialInputUnits(PyObject *self, PyObject *args)
{	
	return PyLong_FromLong(krui_getNoOfSpecialInputUnits());
}	

static PyObject *
snns_getNoOfSpecialOutputUnits(PyObject *self, PyObject *args)
{	
	return PyLong_FromLong(krui_getNoOfSpecialOutputUnits());
}	

static PyObject *
snns_getFirstUnit(PyObject *self, PyObject *args)
{	
	return PyLong_FromLong(krui_getFirstUnit());
}	

static PyObject *
snns_getNextUnit(PyObject *self, PyObject *args)
{	
	return PyLong_FromLong(krui_getNextUnit());
}	

static PyObject *
snns_setCurrentUnit(PyObject *self, PyObject *args)
{	
	return snns_int_arg_with_err(args,krui_setCurrentUnit);
}	

static PyObject *
snns_getCurrentUnit(PyObject *self, PyObject *args)
{	
	return PyLong_FromLong(krui_getCurrentUnit());
}	

typedef char * (*snns_int_arg_char_ret_func)(int);

static PyObject *
snns_int_arg_char_ret(PyObject *arg, snns_int_arg_char_ret_func func)
{
	int i;
	char *ret;
	i = PyLong_AsLong(arg);
	if(PyErr_Occurred()) return NULL;
	ret = func(i);
	if(ret) return PyUnicode_FromString(ret);
	else return Py_BuildValue("");
}	

static PyObject *
snns_getUnitName(PyObject *self, PyObject *args)
{
	return snns_int_arg_char_ret(args,krui_getUnitName);
}	

typedef krui_err (*snns_int_char_args_with_err_func)(int, char *);

static PyObject *
snns_int_char_args_with_err(PyObject *args, snns_int_char_args_with_err_func func)
{
	krui_err err;
	int i;
	char *c;
	if(!PyArg_ParseTuple(args,"is",&i,&c)) return NULL;
	err = func(i,c);
	if(err) return make_exception(err);
	else return Py_BuildValue("");
}	

static PyObject *
snns_setUnitName(PyObject *self, PyObject *args)
{
	return snns_int_char_args_with_err(args,krui_setUnitName);
}

static PyObject *
snns_searchUnitName(PyObject *self, PyObject *args)
{	
	char *a;
	a = (char *)  PyUnicode_AsUTF8(args);
	if(PyErr_Occurred()) return NULL;
	return PyLong_FromLong(krui_searchUnitName(a));
}	

static PyObject *
snns_searchNextUnitName(PyObject *self, PyObject *args)
{	
	return PyLong_FromLong(krui_searchNextUnitName());
}	

static PyObject *
snns_getUnitOutFuncName(PyObject *self, PyObject *args)
{
	return snns_int_arg_char_ret(args,krui_getUnitOutFuncName);
}	

static PyObject *
snns_getUnitActFuncName(PyObject *self, PyObject *args)
{
	return snns_int_arg_char_ret(args,krui_getUnitActFuncName);
}	

static PyObject *
snns_getUnitFTypeName(PyObject *self, PyObject *args)
{
	return snns_int_arg_char_ret(args,krui_getUnitFTypeName);
}	

static PyObject *
snns_setUnitOutFunc(PyObject *self, PyObject *args)
{
	return snns_int_char_args_with_err(args,krui_setUnitOutFunc);
}

static PyObject *
snns_setUnitActFunc(PyObject *self, PyObject *args)
{
	return snns_int_char_args_with_err(args,krui_setUnitActFunc);
}

static PyObject *
snns_getUnitActivation(PyObject *self, PyObject *args)
{	
	int i;
	i = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	return PyFloat_FromDouble(krui_getUnitActivation(i));
}	

static PyObject *
snns_getUnitInitialActivation(PyObject *self, PyObject *args)
{	
	int i;
	i = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	return PyFloat_FromDouble(krui_getUnitInitialActivation(i));
}	

static PyObject *
snns_getUnitOutput(PyObject *self, PyObject *args)
{	
	int i;
	i = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	return PyFloat_FromDouble(krui_getUnitOutput(i));
}	

static PyObject *
snns_getUnitBias(PyObject *self, PyObject *args)
{	
	int i;
	i = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	return PyFloat_FromDouble(krui_getUnitBias(i));
}	

static PyObject *
snns_setUnitActivation(PyObject *self, PyObject *args)
{
	krui_err err;
	int i;
	double d;
	if(!PyArg_ParseTuple(args,"id",&i,&d)) return NULL;
	err = krui_setUnitActivation(i,d);
	if(err) return make_exception(err);
	return Py_BuildValue("");
}	

static PyObject *
snns_setUnitInitialActivation(PyObject *self, PyObject *args)
{
	/*  krui_err err;  */
	int i;
	double d;
	if(!PyArg_ParseTuple(args,"id",&i,&d)) return NULL;
	krui_setUnitInitialActivation(i,d);
	return Py_BuildValue("");
}	

static PyObject *
snns_setUnitBias(PyObject *self, PyObject *args)
{
	/* krui_err err; */
	int i;
	double d;
	if(!PyArg_ParseTuple(args,"id",&i,&d)) return NULL;
	krui_setUnitBias(i,d);
	return Py_BuildValue("");
}	

static PyObject *
snns_setUnitOutput(PyObject *self, PyObject *args)
{
	krui_err err;
	int i;
	double d;
	if(!PyArg_ParseTuple(args,"id",&i,&d)) return NULL;
	err = krui_setUnitOutput(i,d);
	if(err) return make_exception(err);
	return Py_BuildValue("");
}	

static PyObject *
snns_getUnitSubnetNo(PyObject *self, PyObject *args)
{
	int ret,unit;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_getUnitSubnetNo(unit);
	return PyLong_FromLong(ret);
}

static PyObject *
snns_getUnitLayerNo(PyObject *self, PyObject *args)
{
	int ret,unit;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_getUnitLayerNo(unit);
	return PyLong_FromLong(ret);
}

static bool short_check(int i)
{
	short s;
	s = i;
	if(i != s) {
		PyErr_SetString(PyExc_RuntimeError,
		 "expecting a 16 bit integer as second parameter");
		 return FALSE;
	} else return TRUE;
}	


static PyObject *
snns_setUnitSubnetNo(PyObject *self, PyObject *args)
{
	int a,b;
	if(!PyArg_ParseTuple(args,"ii",&a,&b)) return NULL;
	if(!short_check(b)) return NULL;
	krui_setUnitSubnetNo(a,b);
	return Py_BuildValue("");
}

static PyObject *
snns_setUnitLayerNo(PyObject *self, PyObject *args)
{
	int a,b;
	if(!PyArg_ParseTuple(args,"ii",&a,&b)) return NULL;
	if(!short_check(b)) return NULL;
	krui_setUnitLayerNo(a,b);
	return Py_BuildValue("");
}

static bool fillPosType(PyObject *arg, struct PosType *pos)
{
	int i;
	if(!PySequence_Check(arg)) return FALSE;
	if(PySequence_Size(arg) != 3) {
		PyErr_SetString(PyExc_RuntimeError,
		 "Expecting three values for a position definition");
		 return FALSE;
	}
	i = PyLong_AsLong(PySequence_GetItem(arg,0));
	if(!short_check(i)) return FALSE;
	pos->x = i;
	i = PyLong_AsLong(PySequence_GetItem(arg,1));
	if(!short_check(i)) return FALSE;
	pos->y = i;
	i = PyLong_AsLong(PySequence_GetItem(arg,2));
	if(!short_check(i)) return FALSE;
	pos->z = i;
	return TRUE;
}

static PyObject *
buildPosType(struct PosType *pos)
{
	return Py_BuildValue("(iii)",pos->x, pos->y, pos->z);
}

static PyObject *
snns_getUnitPosition(PyObject *self, PyObject *args)
{
	int unit;
	struct PosType pos;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	krui_getUnitPosition(unit, &pos);
	return buildPosType(&pos);
}

static PyObject *
snns_setUnitPosition(PyObject *self, PyObject *args)
{
	int unit;
	struct PosType pos;
	PyObject *seq;
	if(!PyArg_ParseTuple(args,"iO",&unit,&seq) ||
	   !fillPosType(seq,&pos)) return NULL;
	krui_setUnitPosition(unit, &pos);
	return Py_BuildValue("");
}

static PyObject *
snns_getUnitNoAtPosition(PyObject *self, PyObject *args)
{
	struct PosType pos;
	int net;
	PyObject *seq;
	if(!PyArg_ParseTuple(args,"Oi",&seq, &net) ||
	   !short_check(net) || !fillPosType(seq,&pos)) return NULL;
	return PyLong_FromLong(krui_getUnitNoAtPosition(&pos,net));   
}

static PyObject *
snns_getUnitNoNearPosition(PyObject *self, PyObject *args)
{
	struct PosType pos;
	int net,range,width;
	PyObject *seq;
	if(!PyArg_ParseTuple(args,"Oiii",&seq, &net,&range,&width) ||
	   !short_check(net) || !fillPosType(seq,&pos)) return NULL;
	return PyLong_FromLong(krui_getUnitNoNearPosition(&pos,net,range,width));   
}

static PyObject *
snns_getUnitCenters(PyObject *self, PyObject *args)
{
	struct PositionVector *pv;
	int a,b;
	krui_err err;
	if(!PyArg_ParseTuple(args,"ii",&a,&b)) return NULL;
	err = krui_getUnitCenters(a,b,&pv);
	if(err) return make_exception(err);
	else return Py_BuildValue("(ffff)",pv->x, pv->y, pv->z, pv->w);
}

static PyObject *
snns_setUnitCenters(PyObject *self, PyObject *args)
{
	struct PositionVector pv;
	int a,b;
	krui_err err;
	if(!PyArg_ParseTuple(args,"ii(ffff)",&a,&b,&pv.x,
		&pv.y, &pv.z, &pv.w)) return NULL;
	err = krui_setUnitCenters(a,b,&pv);
	if(err) return make_exception(err);
	else return Py_BuildValue("");
}

static PyObject *
snns_getUnitTType(PyObject *self, PyObject *args)
{
	int ret,unit;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_getUnitTType(unit);
	return PyLong_FromLong(ret);
}

static PyObject *
snns_isUnitFrozen(PyObject *self, PyObject *args)
{
	int ret,unit;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_isUnitFrozen(unit);
	return PyLong_FromLong(ret);
}

static PyObject *
snns_getUnitInputType(PyObject *self, PyObject *args)
{
	int ret,unit;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_getUnitInputType(unit);
	return PyLong_FromLong(ret);
}

static PyObject *
snns_freezeUnit(PyObject *self, PyObject *args)
{
	return snns_int_arg_with_err(args,krui_freezeUnit);
}

static PyObject *
snns_unfreezeUnit(PyObject *self, PyObject *args)
{
	return snns_int_arg_with_err(args,krui_unfreezeUnit);
}

static PyObject *
snns_getUnitValueA(PyObject *self, PyObject *args)
{
	int unit;
	double ret;
	unit = PyLong_AsLong(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_getUnitValueA(unit);
	return PyFloat_FromDouble(ret);
}

static PyObject *
snns_setUnitTType(PyObject *self, PyObject *args)
{
	int a,b;
	krui_err err;
	if(!PyArg_ParseTuple(args,"ii",&a,&b)) return NULL;
	err = krui_setUnitTType(a,b);
	if(err) return make_exception(err);
	else return Py_BuildValue("");
}

static PyObject *
snns_setUnitValueA(PyObject *self, PyObject *args)
{
	int a;
	double d;
	if(!PyArg_ParseTuple(args,"if",&a,&d)) return NULL;
	krui_setUnitValueA(a,d);
	return Py_BuildValue("");
}

static PyObject *
snns_createDefaultUnit(PyObject *self, PyObject *args)
{
	int ret;
	ret = krui_createDefaultUnit();
	if(ret < 0) return make_exception(ret);
	else return PyLong_FromLong(ret);
}

static PyObject *
snns_createUnit(PyObject *self, PyObject *args)
{
	char *uname, *fname, *actname;
	int ret;
	double a,b;
	if(!PyArg_ParseTuple(args,"sssff",&uname,&fname,&actname,&a,&b)) return NULL;
	ret=krui_createUnit(uname,fname,actname,a,b);
	if(ret < 0) return make_exception(ret);
	else return PyLong_FromLong(ret);
}

static PyObject *
snns_setUnitFType(PyObject *self, PyObject *args)
{
	return snns_int_char_args_with_err(args,krui_setUnitFType);
}

static PyObject *
snns_copyUnit(PyObject *self, PyObject *args)
{
	int a,b,ret;
	if(!PyArg_ParseTuple(args,"ii",&a,&b)) return NULL;
	ret = krui_copyUnit(a,b);
	if(ret < 0) return make_exception(ret);
	else return PyLong_FromLong(ret);
}

static PyObject *
snns_createFTypeUnit(PyObject *self, PyObject *args)
{
	int ret;
	char *name;
	name = (char * ) PyUnicode_AsUTF8(args);
	if(PyErr_Occurred()) return NULL;
	ret = krui_createFTypeUnit(name);
	if(ret < 0) return make_exception(ret);
	else return PyLong_FromLong(ret);
}

static PyObject *
snns_deleteUnitList(PyObject *self, PyObject *args)
{
	int *array;
	krui_err err;
	int size;
	PyObject *ret=NULL, *tmp=NULL;
	if(PySequence_Size(args) < 1) return Py_BuildValue("");
	tmp = PySequence_GetItem(args,0);	
	if(PySequence_Check(tmp)) {
		if(PySequence_Size(args) > 1) {
			PyErr_SetString(PyExc_RuntimeError,
			 "sorry, no nested types here!");
			 return NULL;
		}
		args = tmp;
	}
	if(!PySequence_Check(args)) {
		PyErr_SetString(PyExc_RuntimeError,
		 "expecting a sequence of unit numbers");
		return NULL;
	}
	size=PySequence_Size(args);
	array = PyMem_New(int,size);
	if(!array) return NULL;
	if(fill_int_array(args,array)) {
		err = krui_deleteUnitList(size,array);
		if(err) make_exception(err);
		else ret = Py_BuildValue("");
	}
	PyMem_Del(array);
	return ret;
}

static PyObject *
snns_getUnitDefaults(PyObject *self, PyObject *args)
{
	FlintType act, bias;
	int iot, subnet, layer;
	char *actfunc, *outfunc;
	krui_getUnitDefaults(&act, &bias, &iot, &subnet, &layer,
		&actfunc, &outfunc);
	return Py_BuildValue("(ffiiiss)",act,bias,iot,subnet,layer,
		actfunc,outfunc);
}

static PyObject *snns_setUnitDefaults(PyObject *self, PyObject *args)
{
	FlintType act, bias;
	int iot, subnet, layer;
	char *actfunc, *outfunc;
	krui_err err;
	if(!PyArg_ParseTuple(args,"ffiiiss", &act, &bias, &iot, &subnet, &layer, &actfunc, &outfunc))
	       	return NULL;
	err = krui_setUnitDefaults(act, bias, iot, subnet, layer, actfunc, outfunc);
	if(err)
	       	return make_exception(err);
	else
	       	return Py_BuildValue("");
}

static PyMethodDef MylibMethods[] = {
     {"getUnitDefaults",snns_getUnitDefaults,METH_NOARGS,
      "getUnitDefaults() -> (activation, bias, io_type, subnet number,\n"
      "                      layer number, activation function, "
      "output function)\n\n"
      "Returns the default values for generating units" },
      
     {"setUnitDefaults",snns_setUnitDefaults,METH_VARARGS,
      "setUnitDefaults(activation, bias, io_type, subnet number, "
      "layer number,\n" 
      "                activation function, output function)\n\n"
      "Sets the default values for generating units" },
      
     {"deleteUnitList",snns_deleteUnitList,METH_VARARGS,
      "deleteUnitList(sequence of unit numbers)\n\n"
      "Deletes all units given in the sequence"},
     
     {"createFTypeUnit",snns_createFTypeUnit,METH_O,
      "createFTypeUnit(ftype name) -> unit number\n\n"
      "Creates a unit with the properties of the (previously defined) "
      "prototype"},
    
     {"copyUnit",snns_copyUnit, METH_VARARGS,
      "copyUnit(unit number,copy mode) -> unit number\n\n"
      "Copies a unit according to the copy mode which determines if links\n"
      "are copied. Available modes:\n"
      "INPUTS_AND_OUTPUTS, ONLY_INPUTS, ONLY_OUTPUTS, ONLY_UNIT"},

     {"setUnitFType",snns_setUnitFType, METH_VARARGS,
      "setUnitFtype(unit number, ftype name)\n\n"
      "Changes the structure of the given unit to the intersection of the\n"
      "current type of the unit with the prototype (ftype)"},
     
     {"createUnit",snns_createUnit, METH_VARARGS,
      "createUnit(name, output function, activation function,\n"
      "           activation, bias) -> unit number\n\n"
      "Creates a unit with the given name, output function, activation\n"
      "function, activation and bias"},
     
     {"createDefaultUnit",snns_createDefaultUnit, METH_NOARGS,
      "createDefaultUnit() -> unit number\n\n"
      "Creates a unit with the default values of the kernel"},
     
     {"setUnitTType",snns_setUnitTType,METH_VARARGS,
      "setUnitTType(unit number, io_type)\n\n"
      "Sets the IO-Type (aka T-type) of the given unit"},
     
     {"freezeUnit",snns_freezeUnit,METH_O,
      "freezeUnit(unit number)\n\n"
      "Freezes the output and activation value of the given unit"},
     
     {"unfreezeUnit",snns_unfreezeUnit,METH_O,
      "unfreezeUnit(unit number)\n\n"
      "Unfreezes the output and activation value of the given unit"},
     
     {"getUnitValueA",snns_getUnitValueA,METH_O,
      "getUnitValueA(unit number) -> float\n\n" 
      "Gets the value A field of the given unit"},

     {"setUnitValueA",snns_setUnitValueA,METH_O,
      "setUnitValueA(???? )\n\n"
      "no estaba definida DAN " },
     
     {"getUnitTType",snns_getUnitTType,METH_O,
      "getUnitTType(unit number) -> io_type\n\n"
      "Gets the IO-Type (aka T-type) of the given unit.\n"
      "Textual representations can be found in the\n"
      "snns.util.unit_types dictionary."},
     
     {"isUnitFrozen",snns_isUnitFrozen,METH_O,
      "isUnitFrozen(unit number) -> boolean\n\n"
      "Checks if the given unit is frozen"},
     
     {"getUnitInputType",snns_getUnitInputType,METH_O,
      "getUnitInputType(unit number)\n\n"
      "Gets the input type of the unit.\n"
      "Textual representations can be found in the\n"
      "snns.util.unit_input_types dictionary."},
     
     {"setUnitCenters",snns_setUnitCenters, METH_VARARGS,
      "setUnitCenters(unit number, center number,\n"
      "               sequence of four float values giving position)\n\n"
      "Sets the 3D transformation center of the specified unit and center\n"
      "number. (Currently the only valid center number seems to be 0)."},
     
     {"getUnitCenters",snns_getUnitCenters, METH_VARARGS,
      "getUnitCenters(unit number, center number) -> sequence of four\n"
      "float values giving the position\n\n"
      "Returns the 3D transformation center of the specified unit and center\n"
      "number. (Currently the only valid center number seems to be 0)."},
     
     {"getUnitNoNearPosition",snns_getUnitNoNearPosition, METH_VARARGS,
      "getUnitNoNearPosition(integer sequence giving position, subnet number,\n"
      "                      range, grid width) -> unit number\n\n"
      "Gets the number of the unit near the given position and in the given\n"
      "subnet. 'Near' is defined by a range and a grid width."},
     
     {"getUnitNoAtPosition",snns_getUnitNoAtPosition, METH_VARARGS,
      "getUnitNoAtPosition(integer sequence giving position,\n"
      "                    subnet number) -> unit number\n\n"
      "Gets the number of the unit at the given position and in the given\n"
      "subnet"},
     
     {"setUnitPosition",snns_setUnitPosition, METH_VARARGS,
      "setUnitPosition(unit number, integer sequence giving position)\n\n"
      "Sets the graphical position of the given unit"},
     
     {"getUnitPosition",snns_getUnitPosition, METH_O,
      "getUnitPosition(unit number) -> integer sequence giving position\n\n"
      "Gets the graphical position of the given unit"},
     
     {"setUnitSubnetNo",snns_setUnitSubnetNo, METH_VARARGS,
      "setUnitSubnetNo(unit number, subnet number)\n\n"
      "Sets the subnet number of the given unit (16 bit range)"},
     
     {"setUnitLayerNo",snns_setUnitLayerNo, METH_VARARGS,
      "setUnitLayerNo(unit number, layer number)\n\n"
      "Sets the layer number of the given unit (16 bit range)"},
     
     {"getUnitLayerNo",snns_getUnitLayerNo,METH_O,
      "getUnitLayerNo(unit number) -> layer number\n\n"
      "Gets the layer number of the unit"},
     
     {"getUnitSubnetNo",snns_getUnitSubnetNo,METH_O,
      "getUnitSubnetNo(unit number) -> subnet number\n\n"
      "Gets the subnet number of the unit"},
     
     {"setUnitInitialActivation",snns_setUnitInitialActivation, METH_VARARGS,
      "setUnitInitialActivation(unit number, initial activation value)\n\n"
      "Sets the initial activation of the given unit to the given value"},
     
     {"setUnitActivation",snns_setUnitActivation, METH_VARARGS,
      "setUnitActivation(unit number, activation value)\n\n"
      "Sets the activation of the given unit to the given value"},
     
     {"setUnitOutput",snns_setUnitOutput, METH_VARARGS,
      "setUnitOutput(unit number, output value)\n\n"
      "Sets the output of the given unit to the given value"},
     
     {"setUnitBias",snns_setUnitBias, METH_VARARGS,
      "setUnitBias(unit number, bias value)\n\n"
      "Sets the bias of the given unit to the given value"},
     
     {"getUnitInitialActivation",snns_getUnitInitialActivation, METH_O,
      "getUnitInitialActivation(unit number) -> initial activation value\n\n"
      "Gets the initial activation value of the given unit"},
     
     {"getUnitOutput",snns_getUnitOutput, METH_O,
      "getUnitOutput(unit number) -> output value\n\n"
      "Gets the output value of the given unit"},
     
     {"getUnitBias",snns_getUnitBias, METH_O,
      "getUnitBias(unit number) -> bias value\n\n"
      "Gets the bias value of the given unit"},
     
     {"getUnitActivation",snns_getUnitActivation, METH_O,
      "getUnitActivation(unit number) -> activation value\n\n"
      "Gets the activation value of the given unit"},
     
     {"setUnitActFunc",snns_setUnitActFunc, METH_VARARGS,
      "setUnitActFunc(unit number, activation function)\n\n"
      "Sets the activation function of the given unit to the given value"},
     
     {"setUnitOutFunc",snns_setUnitOutFunc, METH_VARARGS,
      "setUnitOutFunc(unit number, output function)\n\n"
      "Sets the output function of the given unit to the given value"},
     
     {"getUnitOutFuncName",snns_getUnitOutFuncName, METH_O,
      "getUnitOutFuncName(unit number) -> output function\n\n"
      "Gets the name of the output function of the given unit"},
     
     {"getUnitActFuncName",snns_getUnitActFuncName, METH_O,
      "getUnitActFuncName(unit number) -> activation function\n\n"
      "Gets the name of the activation function of the given unit"},
     
     {"getUnitFTypeName",snns_getUnitFTypeName, METH_O,
      "getUnitFTypeName(unit number) -> f-type\n\n"
      "Gets the name of the f-type of the given unit"},
     
     {"searchNextUnitName",snns_searchNextUnitName, METH_NOARGS,
      "searchNextUnitName() -> unit number\n\n"
      "Searches for the next unit with the name of the last searchUnitName\n"
      "call. Returns 0 if no unit with that name is found."},
     
     {"searchUnitName",snns_searchUnitName, METH_O,
      "searchUnitName(unit name) -> unit number\n\n"
      "Searches for a unit with the given name. Returns 0 if no unit by that\n"
      "name is found."},
     
     {"setUnitName",snns_setUnitName, METH_VARARGS,
      "setUnitName(unit number, name)\n\n"
      "Sets the name of the given unit to the given value"},
     
     {"getUnitName",snns_getUnitName, METH_O,
      "getUnitName(unit number) -> name\n\n"
      "Gets the name of the given unit"},
     
     {"getCurrentUnit",snns_getCurrentUnit, METH_NOARGS,
      "getCurrentUnit() -> unit number\n\n"
      "Gets the number of the current unit or 0"},
     
     {"setCurrentUnit",snns_setCurrentUnit, METH_O,
      "setCurrentUnit(unit number)\n\n"
      "Makes the given unit number the current unit"},
     
     {"getNextUnit",snns_getNextUnit, METH_NOARGS,
      "getNextUnit() -> unit number\n\n"
      "Selects the next unit in the network and makes it current"},
     
     {"getFirstUnit",snns_getFirstUnit, METH_NOARGS,
      "getFirstUnit() -> unit number\n\n"
      "Selects the first unit of the network and makes it current"},
     
     {"getNoOfUnits",snns_getNoOfUnits, METH_NOARGS,
      "getNoOfUnits() -> number of units\n\n"
      "Get the number of units in the neural net"},
     
     {"getNoOfSpecialInputUnits",snns_getNoOfSpecialInputUnits, METH_NOARGS,
      "getNoOfSpecialInputUnits() -> number of special input units\n\n"
      "Get the number of special input units in the neural net"},
     
     {"getNoOfSpecialOutputUnits",snns_getNoOfSpecialOutputUnits, METH_NOARGS,
      "getNoOfSpecialOutputUnits() -> number of special output units\n\n"
      "Get the number of special output units in the neural net"},
     
     {"createSiteTableEntry",snns_createSiteTableEntry, METH_VARARGS,
      "createSiteTableEntry(site name, site function)\n\n"
      "Defines the correspondence between site function and name of the site"},
     
     {"changeSiteTableEntry",snns_changeSiteTableEntry, METH_VARARGS,
      "changeSiteTableEntry(old site name, new site name, new site function)\n\n"
      "Changes the correspondence between site function and name of site"},
     
     {"deleteSiteTableEntry",snns_deleteSiteTableEntry, METH_O,
      "deleteSiteTableEntry(site name)\n\n"
      "Deletes a site in the site table"},
     
     {"getSiteTable",snns_getSiteTable, METH_NOARGS,
      "getSiteTable() -> {site name : site function}\n\n"
      "Returns a dictionary of site names and site functions"},	     
     
     {"getSiteTableFuncName",snns_getSiteTableFuncName, METH_O,
      "getSiteTableFuncName(site name) -> site function\n\n"
      "Returns the name of the site function assigned to the site or None"},
     
     {"setFirstSite",snns_setFirstSite,METH_NOARGS,
      "setFirstSite() -> boolean\n\n"
      "Initializes the first site at the current unit.\n"
      "If there is no site, 0 is returned."},	     
     
     {"setNextSite",snns_setNextSite,METH_NOARGS,
      "setNextSite() -> boolean\n\n"
      "Initializes the next site of the current unit.\n"
      "If there is no next site, 0 is returned."},	     
     
     {"setSite",snns_setSite, METH_O,
      "setSite(site name)\n\n"
      "Initializes the given site of the current unit"},
     
     {"getSiteValue",snns_getSiteValue, METH_NOARGS,
      "getSiteValue() -> site function\n\n"
      "Returns the value of the site function of the current site"},
     
     {"getSiteFuncName",snns_getSiteFuncName, METH_NOARGS,
      "getSiteFuncName() -> site function\n\n"
      "Returns the name of the site function of the current site"},
     
     {"setSiteName",snns_setSiteName, METH_O,
      "setSiteName(site name)\n\n"
      "Changes the name (and thereby the site function) of the current site.\n"
      "The f-type of the unit is erased."},
     
     {"getSiteName",snns_getSiteName, METH_NOARGS,
      "getSiteName() -> site name\n\n"
      "Returns the name of the the current site"},
     
     {"addSite",snns_addSite, METH_O,
      "addSite(site name)\n\n"
      "Adds a new site to the current unit. The new site is inserted in front."},
     
     {"deleteSite",snns_deleteSite, METH_NOARGS,
      "deleteSite() -> boolean\n\n"
      "Deletes the current site of the current unit and all input connections\n"
      "to that site. The f-type of the unit is also erased.\n"
      "If more sites are available, the next one is made current and 1 is\n"
      "returned."},
     
     {"getFirstPredUnit",snns_getFirstPredUnit,METH_NOARGS,
      "getFirstPredUnit() -> (unit number, connection strength)\n\n"
      "Determines predecessor unit number and connection strength of current\n"
      "unit and site.\n"
      "If a predecessor unit exists (number not 0), the connection becomes\n"
      "the current connection."},
     
     {"getFirstPredUnitAndData",snns_getFirstPredUnitAndData,METH_NOARGS,
      "getFirstPredUnitAndData() -> (unit number, connection strength,\n"
      "                              internal 1, internal 2, internal 3)\n\n"
      "Like getFirstPredUnit, but also returns three internal values."},
     
     {"getNextPredUnit",snns_getNextPredUnit,METH_NOARGS,
      "getNextPredUnit() -> (unit number, connection strength)\n\n"
      "Determines another predecessor unit number and connection strength\n"
      "of current unit and site.\n"
      "Otherwise like getFirstPredUnit()"},
     
     {"getNextPredUnitAndData",snns_getNextPredUnitAndData,METH_NOARGS,
      "getNextPredUnitAndData() -> (unit number, connection strength,\n"
      "                             internal 1, internal 2, internal 3)\n\n"
      "Like getNextPredUnit, but also returns three internal values."},
     
     {"getCurrentPredUnit",snns_getCurrentPredUnit,METH_NOARGS,
      "getCurrentPredUnit() -> (unit number, connection strength)\n\n"
      "Gets the current predecessor unit number and connection strength."},
     
     {"getFirstSuccUnit",snns_getFirstSuccUnit,METH_O,
      "getFirstSuccUnit() -> (unit number, connection strength)\n\n"
      "Gets unit number and connection strength of the first successor."},
     
     {"getNextSuccUnit",snns_getNextSuccUnit,METH_NOARGS,
      "getNextSuccUnit() -> (unit number, connection strength)\n\n"
      "Gets unit number and connection strength of the next successor."},
     
     {"isConnected",snns_isConnected,METH_O,
      "isConnected(unit number) -> boolean\n\n"
      "Checks if there is a connection between the current unit and the "
      "given unit."},
     
     {"areConnected",snns_areConnected,METH_VARARGS,
      "areConnected(unit number, unit number) -> weight/None\n\n"
      "Checks if the given two units are connected, returns connection weight "
      "or None"},
     
     {"getLinkWeight",snns_getLinkWeight,METH_NOARGS,
      "getLinkWeight() -> weight\n\n"
      "Gets the connection weight of the current link"},
     
     {"setLinkWeight",snns_setLinkWeight,METH_O,
      "setLinkWeight(weight)\n\n"
      "Sets the connection weight of the current link"},
     
     {"createLink",snns_createLink,METH_VARARGS,
      "createLink(unit number, weight)\n\n"
      "Creates a new link between the current unit/site and the given unit\n"
      "with a given weight."},
     
     {"createLinkWithAdditionalParameters",snns_createLinkWithAdditionalParameters,METH_VARARGS,
      "createLinkWithAdditionalParameters(unit number, weight,\n"
      "                                   internal a, internal b, internal c)"
      "\n\n"
      "Like createLink, but also setting the three internal parameters"},
     
     {"deleteLink",snns_deleteLink,METH_NOARGS,
      "deleteLink()\n\n"
      "Deletes the current link"},
     
     {"deleteAllInputLinks",snns_deleteAllInputLinks,METH_NOARGS,
      "deleteAllInputLinks()\n\n"
      "Deletes all input links of the current unit/site"},
     
     {"deleteAllOutputLinks",snns_deleteAllOutputLinks,METH_NOARGS,
      "deleteAllOutputLinks()\n\n"
      "Deletes all output links of the current unit/site"},
     
     {"jogWeights",snns_jogWeights,METH_VARARGS,
      "jogWeights(lower limit, upper limit)\n\n"
      "Adds uniform distributed random values in the given interval to the\n"
      "connection weights of the network."},
     
     {"jogCorrWeights",snns_jogCorrWeights,METH_VARARGS,
      "jogCorrWeights(lower limit, upper limit, minimum correlation)\n\n"
      "Adds uniform distributed random values in the given interval not to\n"
      "all but only to connection weights of highly correlated, non-special\n"
      "hidden units."},
     
     {"setFirstFTypeEntry",snns_setFirstFTypeEntry,METH_NOARGS,
      "setFirstFTypeEntry() -> boolean\n\n"
      "Initializes the first prototype and makes it current.\n"
      "Returns false if no entries are available."},
     
     {"setNextFTypeEntry",snns_setNextFTypeEntry,METH_NOARGS,
      "setNextFTypeEntry() -> boolean\n\n"
      "Initializes the next prototype and makes it current.\n"
      "Returns false if no entries are available."},
     
     {"setFTypeEntry",snns_setFTypeEntry, METH_O,
      "setFTypeEntry() -> boolean\n\n"
      "Selects a prototype by name. Returns false if not available."},
     
     {"getFTypeName",snns_getFTypeName,METH_NOARGS,
      "getFTypeName() -> ftype/None\n\n"
      "Returns the name of the current prototype or None."},
     
     {"setFTypeName",snns_setFTypeName,METH_O,
      "setFTypeName(ftype name)\n\n"
      "Changes the name of the current prototype"},
     
     {"getFTypeActFuncName",snns_getFTypeActFuncName,METH_NOARGS,
      "getFTypeActFuncName()\n\n"
      "Gets the name of the activation function of the current prototype"},
     
     {"setFTypeActFunc",snns_setFTypeActFunc, METH_O,
      "setFTypeActFunc(activation function)\n\n"
      "Changes the activation function of the current prototype"},
     
     {"getFTypeOutFuncName",snns_getFTypeOutFuncName, METH_NOARGS,
      "getFTypeOutFuncName()\n\n"
      "Gets the name of the output function of the current prototype"},
     
     {"setFTypeOutFunc",snns_setFTypeOutFunc, METH_O,
      "setFTypeOutFunc(output function)\n\n"
      "Changes the output function of the current prototype"},
     
     {"setFirstFTypeSite",snns_setFirstFTypeSite,METH_NOARGS,
      "setFirstFTypeSite() -> boolean\n\n"
      "Select the first site of the prototype.\n"
      "Returns false if there are none"},
     
     {"setNextFTypeSite",snns_setNextFTypeSite,METH_NOARGS,
      "setNextFTypeSite() -> boolean\n\n"
      "Select the next site of the prototype.\n"
      "Returns false if there are none."},
     
     {"getFTypeSiteName",snns_getFTypeSiteName,METH_NOARGS,
      "getFTypeSiteName() -> site name\n\n"
      "Returns the name of the current prototype site"},
     
     {"setFTypeSiteName",snns_setFTypeSiteName,METH_O,
      "setFTypeSiteName(site name)\n\n"
      "Changes the name of the current prototype site"},
     
     {"createFTypeEntry",snns_createFTypeEntry,METH_VARARGS,
      "createFTypeEntry(ftype name, activation function, output function,\n"
      "                 [optional sequence of site names])\n\n"
      "Defines a prototype for units."},
     
     {"deleteFTypeEntry",snns_deleteFTypeEntry,METH_O,
      "deleteFTypeEntry(ftype name)\n\n"
      "Deletes the specified prototype"},
     
     {"getNoOfFunctions",snns_getNoOfFunctions, METH_NOARGS,
      "getNoOfFunctions() -> number of functions\n\n"
      "Determines the number of available functions"},
     
     {"getFuncInfo",snns_getFuncInfo, METH_O,
      "getFuncInfo(function number) -> (name, type)\n\n"
      "Returns name and function type of a function given by its number.\n"
      "The result when given an invalid number is undefined.\n"
      "Textual representations for the functions types can be found in\n"
      "the snns.util.func_types dictionary."},
     
     {"isFunction",snns_isFunction, METH_VARARGS,
      "isFunction(name, type) -> boolean\n\n"
      "Returns True if the function specified by name and type is a valid\n"
      "function"},

     {"getFuncParamInfo",snns_getFuncParamInfo, METH_VARARGS,
      "getFuncParamInfo(function number) -> (no of input, no of output)\n\n"
      "Returns the number of input and output parameters of the function\n"
      "given by name and type or an empty tuple if the function does not exist"},
     {"allocateUnits",snns_allocateUnits, METH_O,
      "allocateUnits(number of units)\n\n"
      "Allocate units, call this if you know how big the network will be\n"
      "before you create it"},
     
     {"updateSingleUnit",snns_updateSingleUnit, METH_O,
      "updateSingleUnit(unit number)\n\n"
      "Evalutes the net input, the activation and the output value of the\n"
      "specified unit (also for frozen units)"},
     
     {"updateNet",snns_updateNet, METH_VARARGS,
      "updateNet([sequence of/multiple] update parameters)\n\n"
      "Updates the network according to the update function with the\n"
      "given parameters."}, 
     
     {"DefTrainSubPat",snns_DefTrainSubPat, METH_VARARGS,
      "DefTrainSubPat([optional insize, outsize, instep, outstep])\n"
      "                 -> number of subpatterns\n\n"
      "Defines the training scheme\n"
      "insize, outsize, instep, outstep are tuples for the subpattern\n"
      "sizes/step sizes for each dimension"},
     
     {"DefShowSubPat",snns_DefShowSubPat, METH_VARARGS,
      "DefShowSubPat([optional insize, outsize, inpos, outpos])\n\n"
      "Defines the subpattern to be shown with the next showPattern call.\n"
      "insize, outsize, inpos, outpos are tuples for the subpattern\n"
      "sizes/positions for each dimension"},
     
     {"deleteNet",snns_deleteNet, METH_NOARGS,
      "deleteNet()\n\n"
      "Delete the network"},
     
     {"error", snns_error, METH_O,
      "error(error code) -> error message\n\n"
      "Returns an error message depending on the error code"},
     
     {"getUpdateFunc",snns_getUpdateFunc, METH_NOARGS,
      "getUpdateFunc() -> update function\n\n"
      "Returns the name of the current update function"},
     
     {"setUpdateFunc",snns_setUpdateFunc, METH_O,
      "setUpdateFunc(update function)\n\n"
      "Changes the update function"},
     
     {"setLearnFunc",snns_setLearnFunc, METH_O,
      "setLearnFunc(learning function)\n\n"
      "Changes the learning function"},
     
     {"setFFLearnFunc",snns_setFFLearnFunc, METH_O,
      "setFFLearnFunc(subordinate learning function)\n\n"
      "Changes the subordinate learning function"},
     
     {"setPrunFunc",snns_setPrunFunc, METH_O,
      "setPrunFunc(pruning function)\n\n"
      "Changes the pruning function"},
     
     {"getInitialisationFunc",snns_getInitialisationFunc, METH_NOARGS,
      "getInitialisationFunc() -> initialisation function\n\n"
      "Returns the name of the current initialisation function"},
     
     {"setInitialisationFunc",snns_setInitialisationFunc, METH_O,
      "setInitialisationFunc(initialisation function)\n\n"
      "Changes the initialisation function"},
     
     {"initializeNet",snns_initializeNet, METH_VARARGS,
      "initializeNet([sequence of/multiple] initialization parameters)\n\n"
      "Initializes the network with the current initialization function"},
     
     {"getLearnFunc",snns_getLearnFunc, METH_NOARGS,
      "getLearnFunc() -> learning function\n\n"
      "Returns the name of the current learning function"},
     
     {"getFFLearnFunc",snns_getFFLearnFunc, METH_NOARGS,
      "getFFLearnFunc() -> subordinate learning function\n\n"
      "Returns the name of the current subordinate learning function"},
     
     {"getPrunFunc",snns_getPrunFunc, METH_NOARGS,
      "getPrunFunc() -> pruning function\n\n"
      "Returns the name of the current pruning function"},
     
     {"getNoOfTTypeUnits",snns_getNoOfTTypeUnits,METH_O,
      "getNoOfTTypeUnits(io_type) -> number of units\n\n"
      "Returns the no. of units of the specified io_type"},
     
     {"learnAllPatternsFF",snns_learnAllPatternsFF,METH_VARARGS,
      "learnAllPatternsFF([sequence of/multiple] learning parameters)\n"
      "                    -> (sequence of output parameters)\n\n"
      "Learns all patterns using the current learning function and the\n"
      "given learning parameters"},
     
     {"learnAllPatterns",snns_learnAllPatterns,METH_VARARGS,
      "learnAllPatterns([sequence of/multiple] learning parameters)\n"
      "                 -> (sequence of output parameters)\n\n"
      "Learns all patterns using the current learning function and the\n"
      "given learning parameters"},
     
     {"learnSinglePatternFF",snns_learnSinglePatternFF,METH_VARARGS,
      "learnSinglePatternFF(pattern number, (sequence of learning parameters))\n"
      "                     -> (sequence of output parameters)\n\n"
      "Same as learnAllPatternsFF, but teaches only the pattern with the\n"
      "given number"},
     
     {"learnSinglePattern",snns_learnSinglePattern,METH_VARARGS,
      "learnSinglePattern(pattern number, (sequence of learning parameters))\n"
      "                   -> (sequence of output parameters)\n\n"
      "Same as learnAllPatterns, but teaches only the pattern with the\n"
      "given number"},
     
     {"GetPatInfo",snns_GetPatInfo,METH_NOARGS,
      "GetPatInfo() -> (pattern_set_info, pattern_descriptor)\n\n"
      "Returns a pattern_set_info and a pattern_descriptor"},
     
     {"getVersion",snns_getVersion, METH_NOARGS,
      "getVersion() -> version string\n\n"
      "Returns the current version of the SNNS-Kernel"},
     
     {"loadNet",snns_loadNet, METH_O,
      "loadNet(filename) -> network name\n\n"
      "Loads a network file and create a network"},
     
     {"loadNewPatterns",snns_loadNewPatterns, METH_O,
      "loadNewPatterns(filename) -> pattern set number\n\n"
      "Loads a pattern file."},
     
     {"getMemoryManagerInfo",snns_getMemoryManagerInfo, METH_NOARGS,
      "getMemoryManagerInfo() -> (alloc bytes for units, sites, name-table,\n"
      "                           site-table, function-table)\n\n"
      "Returns the number of ALLOCATED (not the number of USED) bytes for\n"
      "the units, sites, name-table, site-table and function-table"},
     
     {"getNetInfo",snns_getNetInfo,METH_NOARGS,
      "getNetInfo() -> (number of sites, number of links,\n"
      "                 number of site table entries,\n"
      "                 number of ftype table entries)\n\n"
      "Returns miscellanous information about the current network."},
     
     {"getNoOfOutputUnits",snns_getNoOfOutputUnits, METH_NOARGS,
      "getNoOfOutputUnits() -> number of output units\n\n"
      "Returns the number of output units"},
     
     {"getNoOfInputUnits",snns_getNoOfInputUnits, METH_NOARGS,
      "getNoOfInputUnits() -> number of input units\n\n"
      "Returns the number of input units"},
     
     {"getNoOfPatterns",snns_getNoOfPatterns, METH_NOARGS,
      "getNoOfPatterns() -> number of patterns\n\n"
      "Returns the number of patterns in the current pattern set"},
     
     {"getTotalNoOfSubPatterns",snns_getTotalNoOfSubPatterns, METH_NOARGS,
      "getTotalNoOfSubPatterns() -> number of subpatterns\n\n"
      "Returns the total number of subpatterns contained in all patterns of the current pattern set"},
     
     {"allocNewPatternSet",snns_allocNewPatternSet,METH_NOARGS,
      "allocNewPatternSet() -> pattern set number\n\n"
      "Allocates a new pattern set, returns its number"},
     
     {"setCurrPatSet",snns_setCurrPatSet,METH_O,
      "setCurrPatSet(pattern set number)\n\n"
      "Chooses the number of the current pattern set"},
     
     {"deletePatSet",snns_deletePatSet,METH_O,
      "deletePatSet(pattern set number)\n\n"
      "Deletes all patterns of the pattern set with the given number"},
     
     {"getSymbolTable",snns_getSymbolTable,METH_NOARGS,
      "getSymbolTable() -> { symbol name : type }\n\n"
      "Returns a dictionary of symbols and symbol types (units, sites...)\n"
      "Textual representations for the types are in the\n"
      "snns.util.nametable_types dictionary."},
     
     {"resetNet",snns_resetNet,METH_NOARGS,
      "resetNet()\n\n"
       "Changes all unit activations to the initial activation values"},
     
     {"deleteAllPatterns",snns_deleteAllPatterns,METH_NOARGS,
      "deleteAllPatterns()\n\n"
      "Deletes all patterns."},
     
     {"shufflePatterns",snns_shufflePatterns,METH_O,
      "shufflePatterns(boolean shuffle on or off)\n\n"
      "Switch shuffling of the pattern order on or off"},
     
     {"shuffleSubPatterns",snns_shuffleSubPatterns,METH_O,
      "shuffleSubPatterns(boolean shuffle on or off)\n\n"
      "Switch shuffling of sub pattern pairs on or off"},
     
     {"deletePattern",snns_deletePattern,METH_NOARGS,
      "deletePattern()\n\n"
      "Deletes the current pattern"},
     
     {"modifyPattern",snns_modifyPattern,METH_NOARGS,
      "modifyPattern()\n\n"
      "Modifies the current pattern (sets to current unit activations)"},
     
     {"newPattern",snns_newPattern,METH_NOARGS,
      "newPattern()\n\n"
      "Creates a new pattern. Switches pattern shuffling off."},
     
     {"getPatternNo",snns_getPatternNo,METH_NOARGS,
      "getPatternNo() -> pattern number\n\n"
      "Gets the number of the current pattern"},
     
     {"setPatternNo",snns_setPatternNo,METH_O,
      "setPatternNo(pattern number)\n\n"
      "Sets the current pattern"},
     
     {"saveNet",snns_saveNet,METH_VARARGS,
      "saveNet(filename, network name)\n\n"
      "Save the network"},
     
     {"setRemapFunc",snns_setRemapFunc,METH_VARARGS,
      "setRemapFunc(remap function name, (sequence of parameters))\n\n"
      "defines the pattern remapping function (by function name) and sets\n"
      "its parameters"},
     
     {"saveNewPatterns",snns_saveNewPatterns,METH_VARARGS,
      "saveNewPatterns(filename, pattern set number)\n\n"
      "Saves a pattern set."}, 
     
     {"saveResultParam",snns_saveResultParam,METH_VARARGS,
      "saveResultParam(filename, boolean create file, min pattern number,\n"
      "                max pattern number, boolean include input,\n"
      "                boolean include output, (sequence of update parameters))\n\n"
      "Saves the network result which depends on the loaded patterns.\n"},
     
     {"showPattern",snns_showPattern,METH_O,
      "showPattern(mode)\n\n"
      "Outputs a pattern on the units of the network with one of the\n"
      "OUTPUT_* modes: OUTPUT_NOTHING stores input patterns into unit\n"
      "activations, OUTPUT_ACT also stores output patterns into output units\n"
      "activations, OUTPUT_OUT also updates the output units outputs."}, 
     
     {"setSeedNo",snns_setSeedNo,METH_O,  
      "setSeedNo(number)\n\n"
      "Initalize the pseudo random generator. 0 reinitializes."},
     
     {"GetShapeOfSubPattern",snns_GetShapeOfSubPattern,METH_O,
      "GetShapeOfSubPattern(number of subpattern)\n"
      "                     -> (insize, outsize, inpos, outpos)\n\n"
      "Gets the shape of the n-th subpattern using the current "
      "set, pattern and training scheme (defined with DefTrainSubPat)"},
     
     {"setClassInfo",snns_setClassInfo,METH_O,
      "setClassInfo(class description)\n\n"
      "Assigns the class description to the current pattern.\n"
      "This will work only when all the patterns in the pattern set carry\n"
      "class information or when the current pattern is the only one in the\n"
      "current pattern set."},
     
     {"useClassDistribution",snns_useClassDistribution,METH_O,
      "useClassDistribution(boolean use it or not)\n\n"
      "Toggles the use of class information during training."},
     
     {"setClassDistribution",snns_setClassDistribution,METH_VARARGS,
      "setClassDistribution([sequence of/multiple] integers)\n\n"
      "Defines the composition of the pattern set. The integers\n"
      "supplied as parameters will determine how often each class will\n"
      "be represented in every training epoch.\n"
      "This will override the distribution implicitely defined by the\n"
      "number of patterns of each class.\n"
      "NOTE: The values are assigned to classes in alphanumeric order\n"
      "of the class names!"},
     
     {"AlignSubPat",snns_AlignSubPat,METH_VARARGS,
      "AlignSubPat(sequence for input position,\n"
      "            sequence for output position) -> subpattern number\n\n"
      "Aligns the given sub pattern (by input and output position sequences)\n"
      "to a valid position which fits the sub pattern training scheme.\n"
      "Returns the subpattern number."},
     
     {"getErrorCode",snns_getErrorCode,METH_NOARGS,
      "getErrorCode() -> error code\n\n"
      "Returns the code of the last error. The string representation\n"
      "can be fetched with error(code)."},
     
     {NULL, NULL, 0, NULL}        /* Sentinel */
};

typedef struct {
	char *name;
	int value;
} charintpair;

static PyObject *funcdict, *utildict;

static PyObject *getCustomFunction(char *funcname, int functype)
{
	PyObject *func;
	func = PyDict_GetItem(funcdict, PyUnicode_FromString(funcname));
	if(!func || ! (func = PyDict_GetItem(func, PyLong_FromLong(functype)))) {
		fputs("Called function not registered, this should "
	                "not happen!\n",stderr);
		return 0;		
	} else {
		func = PyTuple_GetItem(func,2);
		if(!PyCallable_Check(func)) {
			fputs("Oops! Got non-callable as function from dict\n",
				stderr);
		}		
		return func;
	}	
}

static FlintType OutFunctionCaller(PyObject *func, FlintType activation)
{
	PyObject *res;
	double d;
	res = PyObject_CallFunction(func,"d",activation);
	d = PyFloat_AsDouble(res);	
	Py_DECREF(res);
	return d;
}	

static FlintType ActFunctionCaller(PyObject *func, struct Unit *unit_ptr)
{
	PyObject *res;
	double d;
	int unitnum;
	struct Unit *uptr2;
	for(unitnum = 1, uptr2 = unit_array + 1;
	    unitnum <= MaxUnitNo;
	    ++unitnum, ++uptr2) {
	    if(uptr2 == unit_ptr)  break;
	}   
	res = PyObject_CallFunction(func,"i",unitnum);
	d = PyFloat_AsDouble(res);	
	Py_DECREF(res);
	return d;
}	

static int getNoOfFuncs()
{
	int num=0;
	Py_ssize_t  i=0, j;
	PyObject *key, *value, *key2, *value2;
	while(PyDict_Next(funcdict, &i, &key, &value))
       	{
		j=0;
		while(PyDict_Next(value, &j, &key2, &value2))  ++num;
	}	
	return num;
}

static krui_err getFuncInfo(int mode, struct FuncInfoDescriptor *desc)
{
	PyObject *func, *res, *tmpnone;
	char *tmpstr;
	krui_err ret;
	res = NULL ;
	ret = KRERR_PARAMETERS;
	tmpnone = Py_BuildValue("");
	func = PyDict_GetItemString(utildict,"internalGetFuncInfo");
	if(!func) {
		fputs("can't locate Python function 'internalGetFuncInfo'\n",stderr);
	} else {
		res = PyObject_CallFunction(
				func,
				"iisiiiO",
				mode,
				desc->number,
			       	(mode == SEARCH_FUNC || mode == GET_FUNC_NAME) ?  desc->func_name : "",
			       	desc->func_type,
				desc->no_of_input_parameters,
				desc->no_of_output_parameters,
				(mode == GET_FUNC_NAME) ?  desc->function : (FunctionPtr)tmpnone
		);
		if(res && PyArg_ParseTuple(res,"iisiiO",&desc->number,
					&desc->func_type,
					&tmpstr,
					&desc->no_of_input_parameters,
					&desc->no_of_output_parameters,
					(mode == GET_FUNC_NAME) ?
					   tmpnone : 
					   (PyObject *)&desc->function)
			&& desc->number != 0) {
			strncpy(desc->func_name,tmpstr,FUNCTION_NAME_MAX_LEN-1);
			desc->func_name[FUNCTION_NAME_MAX_LEN-1] = 0;
			ret = KRERR_NO_ERROR;	
		}
		Py_DECREF(tmpnone);				
	}
	if(res) Py_DECREF(res);
	return ret;
}

PyMODINIT_FUNC  PyInit_krui(void)
{
	char *moduledoc = 
	"PYTHON3.\n"
	"This module provides all functions and constants of the SNNS kernel.\n"
	"Please note that it is not part of SNNS but distributed seperately.\n"
	"\n"
	"It stays very close to the C API, so you can also refer to the\n"
	"original SNNS documentation for additional hints.\n"
	"\n"
	"If you feel you have to do so because this documentation is unclear,\n"
	"please contact the author of this extension.";
	/* PyObject *m,*dict, *utilmod; */
	PyObject *m, *utilmod;

	charintpair thingtypes[]= {
		/* Unit Types */
		{"UNKNOWN",UNKNOWN},
		{"INPUT",INPUT},
		{"OUTPUT",OUTPUT},
		{"HIDDEN",HIDDEN},
		{"DUAL",DUAL},
		{"SPECIAL",SPECIAL},
		{"SPECIAL_I",SPECIAL_I},
		{"SPECIAL_O",SPECIAL_O},
		{"SPECIAL_H",SPECIAL_H},
		{"SPECIAL_D",SPECIAL_D},
		{"SPECIAL_X",SPECIAL_X},
		{"N_SPECIAL_X",N_SPECIAL_X},
		/* Pattern update modes */
		{"OUTPUT_NOTHING",OUTPUT_NOTHING},
		{"OUTPUT_ACT",OUTPUT_ACT},
		{"OUTPUT_OUT",OUTPUT_OUT},
		/* Function Types */
		{"OUT_FUNC",OUT_FUNC},
		{"ACT_FUNC",ACT_FUNC},
		{"SITE_FUNC",SITE_FUNC},
		{"LEARN_FUNC",LEARN_FUNC},
		{"UPDATE_FUNC",UPDATE_FUNC},
		{"INIT_FUNC",INIT_FUNC},
		{"ACT_DERIV_FUNC",ACT_DERIV_FUNC},
		{"JOG_WEIGHT_FUNC",JOG_WEIGHT_FUNC},
		{"ACT_2_DERIV_FUNC",ACT_2_DERIV_FUNC},
		{"PRUNING_FUNC",PRUNING_FUNC},
		{"TEST_FUNC",TEST_FUNC},
		{"REMAP_FUNC",REMAP_FUNC},
		{"FUNC_TYPE_MASK",FUNC_TYPE_MASK},
		{"DEFAULT_FUNC",DEFAULT_FUNC},
		{"BENCH_FUNC",BENCH_FUNC},
		{"FF_LEARN_FUNC",FF_LEARN_FUNC},
		/* Unit input types */
		{"NO_INPUTS",NO_INPUTS},
		{"SITES",SITES},
		{"DIRECT_LINKS",DIRECT_LINKS},
		/* Copy Modes */
		{"INPUTS_AND_OUTPUTS",INPUTS_AND_OUTPUTS},
		{"ONLY_INPUTS",ONLY_INPUTS},
		{"ONLY_OUTPUTS",ONLY_OUTPUTS},
		{"ONLY_UNIT",ONLY_UNIT},
		/* Name table entries */
		{"UNUSED_SYM",UNUSED_SYM},
		{"UNIT_SYM",UNIT_SYM},
		{"SITE_SYM",SITE_SYM},
		{"FTYPE_UNIT_SYM",FTYPE_UNIT_SYM}, 
		/* get function info modes - only those implemented in
		   snns.util */
		{"SEARCH_FUNC",SEARCH_FUNC},
		{"GET_FUNC_INFO",GET_FUNC_INFO},
		{"GET_FUNC_NAME",GET_FUNC_NAME},
		{NULL,0} /* Finished */
		};
	charintpair *cip;
	char **txt;

	char *patsetinfodoc[] = {
		"number_of_pattern","number of pattern (pairs) in this set",
		"virtual_no_of_pattern","number of pattern (pairs) in this set if class_distrib_active == TRUE",
		"output_present","true if output pattern present",
		"fixed_fixsizes", "true if the fixsizes of all pattern are equal",
		"in_fixsize","if fixed_fixsizes is true, fixsize of the input pattern, else -1",
		"out_fixsize","if fixed_fixsizes is true, fixsize of the input pattern, else -1",
		"in_max_dim_sizes", "maximum values of the input dimensions",
		"out_max_dim_sizes", "maximum values of the output dimensions",
		"in_min_dim_sizes", "minimum values of the input dimensions",
		"out_min_dim_sizes", "minimum values of the output dimensions",
		
		"class_names", "names of available classes",
		"class_redistribution", "amounts for the class redistribution",
		"remap_params", "parameters for the remap function",
		"class_distrib_active", "true if the class redistribution is active",
		"remap_function", "the name of the remap function",
		NULL
	};

	char *patdesdoc[] = {
		"input_dim_sizes", "actual sizes of the input dimensions",
		"input_fixsize", "size of the fixed part in the pattern or 0 if no input pattern is present",
		"output_dim_sizes", "actual sizes of the output dimensions",
		"output_fixsize", "size of the fixed part in the pattern or 0 if no output pattern is present",
		"my_class", "class index of this pattern or -1 if no classes available",
		NULL
	};

	kr_PythonOutFunctionHook = OutFunctionCaller;	
	kr_PythonActFunctionHook = ActFunctionCaller;
	kr_findPythonFunctionHook = getCustomFunction;
	kr_getPythonFuncInfoHook = getFuncInfo;
	kr_getNoOfPythonFunctionsHook = getNoOfFuncs;

	utilmod = PyImport_Import(PyUnicode_FromString("snns.util"));
	if(!utilmod)
	       	return NULL ;
	
	utildict = PyModule_GetDict(utilmod);
	funcdict = PyDict_GetItemString(utildict, "custom_functions");
	if(!funcdict)
	       	return NULL ;
	
	static struct PyModuleDef moduledef = { 
            PyModuleDef_HEAD_INIT, "krui" , "SNNS kernel interface", -1, MylibMethods, 
	};
	
	m = PyModule_Create(&moduledef);
	if ( m == NULL )
		return NULL ; 

	/*  m = Py_InitModule3("krui", MylibMethods,"SNNS kernel interface");  */
	
	for(cip=thingtypes; cip->name; ++cip)
       	{
		PyModule_AddObject(m,cip->name,PyLong_FromLong(cip->value));	
	}

	PyModule_AddObject(m,"__doc__", PyUnicode_FromString(moduledoc));

	snns_pattern_set_info_type.tp_new = PyType_GenericNew;
	
	if (PyType_Ready(&snns_pattern_set_info_type) < 0)
	       	return NULL ;
	Py_INCREF(&snns_pattern_set_info_type);
	
	for(txt=patsetinfodoc; *txt; txt+=2)
       	{
		PyDict_SetItemString(snns_pattern_set_info_type.tp_dict, txt[0], PyUnicode_FromString(txt[1]));
	}
	
	PyModule_AddObject(m,"pattern_set_info",(PyObject *)&snns_pattern_set_info_type);

	snns_pattern_descriptor_type.tp_new = PyType_GenericNew;
	
	if (PyType_Ready(&snns_pattern_descriptor_type) < 0)
	       	return NULL ;
	Py_INCREF(&snns_pattern_descriptor_type);

	for(txt=patdesdoc; *txt; txt+=2)
       	{
		PyDict_SetItemString(snns_pattern_descriptor_type.tp_dict, txt[0], PyUnicode_FromString(txt[1]));
	}

	PyModule_AddObject(m,"pattern_descriptor",(PyObject *)&snns_pattern_descriptor_type);
	return m; 
}


