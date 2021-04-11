if [ "$1" = "--help" ]
then 
	echo "runner.sh <filter> <out_format> <target map>";
else
	cmd="./cmake-build-debug/$3_bench";
	out_format=$2;
	file_format=$2;
	if [ "$out_format" = "console" ]
	then
		file_format="txt"
	fi
	$cmd --benchmark_filter=$1 --benchmark_out="./results/$3/$1.$file_format" --benchmark_out_format=$out_format
fi
