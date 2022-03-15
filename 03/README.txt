Rachel Ng 




for i in Images/*.pgm; do echo ./p1 ${i} h1_${i##*/}; done
for i in Images/*.pgm; do ./p1 ${i} h1_${i##*/}; done


for i in h1_*.pgm;do name=${i##h1_} base=${name%.pgm}; echo ./h2 ${i} 100 binary_${name}; done;
for i in h1_*.pgm;do name=${i##h1_} base=${name%.pgm}; ./h2 ${i} 100 binary_${name}; done;


for i in binary_*.pgm;do name=${i##binary_} base=${name%.pgm}; echo ./h3 ${i} h3_${name} h3_${base}.txt; done;
for i in binary_*.pgm;do name=${i##binary_} base=${name%.pgm}; ./h3 ${i} h3_${name} h3_${base}.txt; done;


for i in h3_*.pgm;do name=${i##h3_} base=${name%.pgm}; echo ./h4 ${i} h3_${base}.txt 180 h4_${name}; done;
for i in h3_*.pgm;do name=${i##h3_} base=${name%.pgm}; ./h4 ${i} h3_${base}.txt 180 h4_${name}; done;


for i in Images/*.pgm;do name=${i##*/} base=${name%.pgm}; echo ./h4 ${i} h3_${base}.txt 10 h4_${name}; done;
for i in Images/*.pgm;do name=${i##*/} base=${name%.pgm}; ./h4 ${i} h3_${base}.txt 10 h4_${name}; done;




