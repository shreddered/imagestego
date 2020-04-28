sum=0

echo 'Source files:'

for var in `find . -type f -name *.cpp`; do
    echo $var
    sum=$(( $sum + `wc -l $var | egrep -o [0-9]+` ))
done

echo 'Headers:'
for var in `find . -type f -name *.hpp`; do
    echo $var
    sum=$(( `wc -l $var | egrep -o [0-9]+` + $sum ))
done

echo $sum
