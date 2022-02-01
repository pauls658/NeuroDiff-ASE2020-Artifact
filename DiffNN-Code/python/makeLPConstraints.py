inputsBuf = "[ 0.600000023842 -0.499999880791 -0.499999880791 0.449999988079 -0.500000000000 ] [ 0.679857730865 0.499999880791 0.499999880791 0.500000000000 -0.449999988079 ]"
inputsBuf = inputsBuf.replace("[ ", "[")
inputsBuf = inputsBuf.replace(" ", ",")
inputsLB, inputsUB = eval(inputsBuf)

def getEq(buf):
	lbBuf, ubBuf = buf.split("] [")
	return lbBuf.strip("["), ubBuf.strip("]")

eqBuf = "[+0.054006200283x1 -2.610919952393x2 -0.180026993156x3 +0.242193996906x4 +0.141406998038x5 +0.227630004286 ] [+0.054006200283x1 -2.610919952393x2 -0.180026993156x3 +0.242193996906x4 +0.141406998038x5 +0.227630004286 ]"
eqPrimeBuf = "[+0.054019998759x1 -2.611000061036x2 -0.180000007153x3 +0.242200002074x4 +0.141399994492x5 +0.227699995040 ] [+0.054019998759x1 -2.611000061036x2 -0.180000007153x3 +0.242200002074x4 +0.141399994492x5 +0.227699995040 ]"
eqDeltaBuf = "[+0.000013798475x1 -0.000080108643x2 +0.000026986002x3 +0.000006005167x4 -0.000007003546x5 +0.000069990754 ] [+0.000013798475x1 -0.000080108643x2 +0.000026986002x3 +0.000006005167x4 -0.000007003546x5 +0.000069990754 ]"

eqLB, eqUB = getEq(eqBuf)
eqPrimeLB, eqPrimeUB = getEq(eqPrimeBuf)
eqDeltaLB, eqDeltaUB = getEq(eqDeltaBuf)

print("max: "  + eqDeltaUB + ";")
for i in range(len(inputsLB)):
    print(repr(inputsLB[i]) + "<= x" + str(i + 1) + " <= " + repr(inputsUB[i]) + ";")

print("nin <= " + eqUB + ";")
print("nin >= " + eqLB + ";")
print("din <= " + eqDeltaUB + ";")
print("din >= " + eqDeltaLB + ";")
print("nin >= 0;")
print("nin + din >= 0;")
