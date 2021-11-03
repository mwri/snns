#!/usr/bin/python3

# shows the coordinates of the winner neurons for the som_cube example

from snns import krui, util

krui.loadNet('som_cube.net')
krui.loadNewPatterns('som_cube.pat')
patnum = krui.getNoOfPatterns()
units = krui.getNoOfUnits()
	
for pat in range(1,patnum+1) :
	krui.setPatternNo(pat)
	krui.showPattern(krui.OUTPUT_NOTHING)
	krui.updateNet(())
	results = []
	for unit in range(1,units+1) :
		if krui.getUnitTType(unit) == krui.HIDDEN :
			results.append((krui.getUnitActivation(unit),unit))
	bestact, bestunit = min(results)
	rawpos = krui.getUnitPosition(bestunit)[:2]
	print("Pattern", pat, "Act", bestact, "Unit", bestunit, end=' ') 
	print("Grid Position", (rawpos[0]-4, rawpos[1])) 
