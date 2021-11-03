#!/usr/bin/python3
# Construct a encoder.net like network from scratch

from snns import krui,util

krui.setLearnFunc("Std_Backpropagation")
krui.setUpdateFunc('Topological_Order')
krui.setUnitDefaults(1,0,krui.INPUT,0,1,'Act_Logistic','Out_Identity')

print ("Creating the network out of thin air")

# build the input layer
pos = [0,0,0]
inputs = []
for i in range(1,9) :
	pos[0] = i
	num = krui.createDefaultUnit()
	inputs.append(num)
	krui.setUnitName(num,'Input_%i' % i)
	krui.setUnitPosition(num, pos)
# hidden layer
pos[1]=2
hidden = []
for i in range(1,4) :
	pos[0] = i + 3
	num = krui.createDefaultUnit()
	hidden.append(num)
	krui.setUnitName(num,'Hidden_%i' % i)
	krui.setUnitTType(num,krui.HIDDEN)
	krui.setUnitPosition(num,pos)
	krui.setCurrentUnit(num)
	for src in inputs :
		krui.createLink(src,0)

# output layer
pos[1]=4
outputs = []
for i in range(1,9) :
	pos[0] = i
	num = krui.createDefaultUnit()
	outputs.append(num)
	krui.setUnitName(num,'Output_%i' % i)
	krui.setUnitTType(num,krui.OUTPUT)
	krui.setUnitPosition(num,pos)
	krui.setCurrentUnit(num)
	for src in hidden :
		krui.createLink(src,0)

print ("Creating patterns")

krui.deleteAllPatterns()
patset = krui.allocNewPatternSet()
for unum in range(8) :
	for curnum in range(8) :
		for row in inputs, outputs :
			krui.setUnitActivation(row[curnum], curnum == unum)
	krui.newPattern()		

krui.initializeNet(-1,1)
krui.shufflePatterns(1)
krui.DefTrainSubPat()

print ("Training")

i=0
while i < 50000 :
	res = krui.learnAllPatterns(0.2,0)
	if not i % 5000 : print ("Error in learning cycle %d:" % i, res[0] )
	i = i + 1

print ("Writing pyencoder.pat")
krui.saveNewPatterns('pyencoder.pat',patset)
print ("Writing pyencoder.net")
krui.saveNet('pyencoder.net','pyencoder')
