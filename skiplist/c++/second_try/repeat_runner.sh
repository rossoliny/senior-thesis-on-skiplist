status=0;

for ((i=1; i<=$2 && status==0; i++))
do
	$1;
	status=$?;
done
