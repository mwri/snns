#!/usr/bin/python3
from snns import krui,util

def outtestfunc(x) :
	print ("first output function:", x)
	return x/2

def outtestfunc2(x) :
	print ("second output function:", x)
	return x/3

def acttest(x) :
	print ("ActTest for unit", x)
	return 0

def actderivtest(x) :
	print ("ActDerivTest for unit", x)
	return 0

def actderiv2test(x) :
	print ("ActDeriv2Test for unit", x)
	return 0


oldnum = krui.getNoOfFunctions()
print ("Builtin functions:", oldnum)
util.registerFunction(outtestfunc,"outtestfunc",krui.OUT_FUNC,0,0)
util.registerFunction(outtestfunc2,"outtestfunc2",krui.OUT_FUNC,0,0)
util.registerFunction(acttest,"acttest",krui.ACT_FUNC,0,0)
util.registerFunction(actderivtest,"acttest",krui.ACT_DERIV_FUNC,0,0)
util.registerFunction(actderiv2test,"acttest",krui.ACT_2_DERIV_FUNC,0,0)
newnum = krui.getNoOfFunctions()
print ("After adding:", newnum)

for num in range(oldnum - 2, newnum + 1) :
	print ("Function number", num, "Info:",krui.getFuncInfo(num))

print (krui.loadNet('encoder.net') )
for num in [1,10,19] :
	krui.setUnitOutFunc(num,"outtestfunc")
for num in [2,11,18] :
	krui.setUnitOutFunc(num,"outtestfunc2")
for num in [3,9,17] :
	krui.setUnitActFunc(num,"acttest")

krui.loadNewPatterns('encoder.pat')
krui.DefTrainSubPat()
print ("Learning one pattern")
krui.learnSinglePattern(1,(0.2,0))
krui.setUnitDefaults(1.0,0,krui.INPUT,0,1,"acttest","outtestfunc")
newunit = krui.createDefaultUnit()
print ("New unit:", newunit)
print ("Act func name:", krui.getUnitActFuncName(newunit) )
krui.deleteUnitList(newunit)
krui.saveNet("tmp.net","testnet")
print ("finished")

