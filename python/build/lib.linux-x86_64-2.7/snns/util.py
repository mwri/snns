"""
Here you find some definitions (and later maybe functions) which should
simplify work with the SNNS kernel. Feel free to suggest additions.
"""

def registerFunction(func, fname, ftype, inparam, outparam) :
	"""
	register a custom Python function for further use with the
	SNNS kernel.
	"""
	if not custom_functions.get(fname) :
		custom_functions[fname]={}
	custom_functions[fname][ftype]=(inparam,outparam,func)

custom_functions = {}
from snns import krui

unit_types = {	krui.UNKNOWN : 'Unknown',
		krui.INPUT : 'Input',
		krui.OUTPUT : 'Output',
		krui.HIDDEN : 'Hidden',
		krui.DUAL : 'Dual',
		krui.SPECIAL : 'Special',
		krui.SPECIAL_I : 'Special Input',
		krui.SPECIAL_O : 'Special Output',
		krui.SPECIAL_H : 'Special Hidden',		krui.SPECIAL_D : 'Special Dual',
		krui.SPECIAL_X : 'Special X',
		krui.N_SPECIAL_X : 'N Special X'}

nametable_types = {	krui.UNUSED_SYM : 'Unused',
		    	krui.UNIT_SYM : 'Unit',
			krui.SITE_SYM : 'Site',
			krui.FTYPE_UNIT_SYM : 'Unit Prototype'}

pattern_update_modes = {
		krui.OUTPUT_NOTHING : 'Output Nothing',
		krui.OUTPUT_ACT : 'Output Act',
		krui.OUTPUT_OUT : 'Output Out'}

unit_input_types = {	krui.NO_INPUTS : 'No inputs',
		    	krui.SITES : 'Sites',
			krui.DIRECT_LINKS : 'Direct links'}

copy_modes = {	krui.INPUTS_AND_OUTPUTS : 'Inputs and outputs',
		krui.ONLY_INPUTS : 'Only inputs',
		krui.ONLY_OUTPUTS : 'Only outputs',
		krui.ONLY_UNIT : 'Only the unit'}

func_types = {	krui.OUT_FUNC : 'Output',
		krui.ACT_FUNC : 'Activation',
		krui.SITE_FUNC : 'Site',
		krui.LEARN_FUNC : 'Learning',
		krui.UPDATE_FUNC : 'Update',
		krui.INIT_FUNC : 'Initialization',
		krui.ACT_DERIV_FUNC : 'Act Deriv',
		krui.JOG_WEIGHT_FUNC : 'Jog Weight',
		krui.ACT_2_DERIV_FUNC : 'Act2 Deriv',
		krui.PRUNING_FUNC : 'Pruning',
		krui.TEST_FUNC : 'Test',
		krui.REMAP_FUNC : 'Remap',
		krui.DEFAULT_FUNC : 'Default',
		krui.BENCH_FUNC : 'Bench',
		krui.FF_LEARN_FUNC : 'Feedforward Learning'}

def internalGetFuncInfo(mode, number, funcname, functype, numberin, numberout, function) :
	"""
	This function is called by the C part of the extension
	and returns the function number (or 0 on error), function name,
	function type, number of input parameters, number of output parameters
	and the function itself.
	
	It works much like the internal kernel function
	krf_getInternalFuncInfo and is called by it.
	"""
	retnumber = 0
	retnumberin = 0
	retnumberout = 0
	rettype = functype
	retfunc = lambda : 0
	retname = "Invalid_Name"

	funclist = []
	for key1 in custom_functions :
		for key2 in custom_functions[key1] :
			funclist.append((key1,key2,
					custom_functions[key1][key2]))
			
	funclist.sort()
	if mode == krui.SEARCH_FUNC :
		try :
			res = custom_functions[funcname][functype]
			retnumber = funclist.index((funcname,functype,res)) + 1
			rettype = functype
			retname = funcname
			retnumberin, retnumberout, retfunc = res
		except :
			pass
	elif mode == krui.GET_FUNC_INFO :
		res = funclist[number - 1 - 
			(krui.getNoOfFunctions() - len(custom_functions))]
		retnumber = number
		retname, rettype,  = res[:2]
		retnumberin, retnumberout, retfunc = res[2]
	elif mode == krui.GET_FUNC_NAME :
		barefunclist = [ f[len(f)-1][2] for f in funclist ]
		try :
			location = barefunclist.index(function)
			res = funclist[location]
			retnumber = location + 1
			retname, rettype = res[:2]
			retnumberin, retnumberout, retfunc = res[2]
		except:
			retnumber = 0
			print ("Could not determine function's name")
	else :
		print ("No such search mode")
		raise ValueError,  "bad search mode"
	return (retnumber,rettype,retname,retnumberin,retnumberout, retfunc)
	
