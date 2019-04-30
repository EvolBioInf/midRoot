./midRoot > tmp.out
DIFF=$(diff tmp.out ../data/midRoot.out)
if [ "$DIFF" == "" ] 
then
    printf "Test(midRoot)\tpass\n"
else
    printf "Test(midRoot)\tfail\n"
    echo ${DIFF}
fi

rm tmp.out
