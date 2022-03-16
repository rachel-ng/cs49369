Rachel Ng 


image.* 
Image class provided by Professor Stamos 

Pixel.h 
Pixel class for points (x,y)

Object.*
Object class for labeled objects 

h1.cc
USAGE: ./h1 input_image output_image 

h2.cc
USAGE: ./h2 input_image threshold binary_image


h3.cc
USAGE: ./h3 input_image output_image voting_array 


h4.cc 
USAGE: ./h4 input_image voting_array threshold output_image



for i in Images/*.pgm; do echo ./h1 ${i} h1_${i##*/}; done
for i in Images/*.pgm; do ./h1 ${i} h1_${i##*/}; done


for i in h1_*.pgm;do name=${i##h1_} base=${name%.pgm}; echo ./h2 ${i} 100 binary_${name}; done;
for i in h1_*.pgm;do name=${i##h1_} base=${name%.pgm}; ./h2 ${i} 100 binary_${name}; done;


for i in binary_*.pgm;do name=${i##binary_} base=${name%.pgm}; echo ./h3 ${i} h3_${name} h3_${base}.txt; done;
for i in binary_*.pgm;do name=${i##binary_} base=${name%.pgm}; ./h3 ${i} h3_${name} h3_${base}.txt; done;


for i in Images/*.pgm;do name=${i##*/} base=${name%.pgm}; echo ./h4 ${i} h3_${base}.txt 10 h4_${name}; done;
for i in Images/*.pgm;do name=${i##*/} base=${name%.pgm}; ./h4 ${i} h3_${base}.txt 10 h4_${name}; done;



./h4 Images/hough_complex_1.pgm h3_hough_complex_1.txt 10 h4_hough_complex_1.pgm
./h4 Images/hough_simple_1.pgm h3_hough_simple_1.txt 10 h4_hough_simple_1.pgm
./h4 Images/hough_simple_2.pgm h3_hough_simple_2.txt 10 h4_hough_simple_2.pgm


