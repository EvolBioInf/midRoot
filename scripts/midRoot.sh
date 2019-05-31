./midRoot ../data/smallU.nwk > tmp.out
DIFF=$(diff tmp.out ../data/smallR.nwk)
if [ "$DIFF" == "" ] 
then
    printf "Test(midRoot)\tpass\n"
else
    printf "Test(midRoot)\tfail\n"
    echo ${DIFF}
fi

rm tmp.out
