status=0;

for ((i=1; i<=$2 && status==0; i++))
do
	printf "\nTest Run #"$i"\n";
	$1;
	status=$?;
done
