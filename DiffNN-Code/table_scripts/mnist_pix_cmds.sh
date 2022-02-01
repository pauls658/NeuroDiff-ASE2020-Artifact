nnet=mnist_relu_4_1024
timeout=1800
eps=1
for prop in $( seq 400 499 ); do
	for x in $( seq 15 3 27 ); do
		echo "./delta_network_test $prop nnet/$nnet.nnet compressed_nnets/${nnet}_16bit.nnet $eps -x $x"
		timeout -s 2 $timeout ./delta_network_test $prop nnet/$nnet.nnet compressed_nnets/${nnet}_16bit.nnet $eps -x $x
	done
done
