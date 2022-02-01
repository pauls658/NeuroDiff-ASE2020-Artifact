#!/bin/bash

uppers=(6 5)
lowers=(4 1)
network="test.nnet"
prop="300"

while true; do
	input=""
	for index in "${!uppers[@]}"; do
		num=$( python3 -c "import random; print(str(
		(random.random()*10.0 % (${uppers[$index]} - ${lowers[$index]})) + ${lowers[$index]}))" )
		input="$input $num"
	done
	#output=$( ./run_network $network $prop $input | grep -o -E "[0-9.]*" )
	o1=$( ./run_network $network $prop $input | grep -o -E "[0-9.]*" ) 
	o2=$( ../ReluVal-for-comparison/run_network ../ReluVal-for-comparison/subbed_nnets/b2layer_0.nnet $prop $input | grep -o -E "[0-9.]*")
	diff=$( python -c "print str(round($o2 - $o1, 4))" )	
	echo $diff
	continue
	if (( $( echo "1.1 > ${output}" | bc -l ) )); then
		echo "Violation found: $input"
		echo "Output: $output"
		exit 0
	fi
done
