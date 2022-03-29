echo "" > f1.txt
echo "" > f2.txt
echo "" > f3.txt
echo "" > f4.txt
echo "" > f5.txt

g++ -Wall ./main.cpp -fopenmp -std=c++11
./a.out

cat *.txt > gathered.txt