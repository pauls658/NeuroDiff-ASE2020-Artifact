nnet=mnist_relu_4_1024
eps=1
for prop in $( seq 400 499 ); do
	for p in $( seq 3 8 ); do
		echo "./delta_network_test $prop nnet/$nnet.nnet compressed_nnets/${nnet}_16bit.nnet $eps -p $p"
		./delta_network_test $prop nnet/$nnet.nnet compressed_nnets/${nnet}_16bit.nnet $eps -p $p -m 0
	done
done
