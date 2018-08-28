for i in *root
do
	if [ -e $i ] ;
		then
		rm $i
	fi
done;

for i in *pcm
do
	if [ -e $i ] ;
		then
		rm $i
	fi
done;

for i in *d
do
	if [ -e $i ] ;
		then
		rm $i
	fi
done;

for i in *so
do
	if [ -e $i ] ;
		then
		rm $i
	fi
done;
