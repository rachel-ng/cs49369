
```bash 
for i in PgmImages/*.pgm; do echo ./p1 ${i} 120  binary_${i##*/}; done;

for i in PgmImages/*.pgm; do ./p1 ${i} 120  binary_${i##*/}; done;
```

```bash 
for i in binary*.pgm;do name=${i##binary_} base=${name%.pgm}; echo ${base}; done;

for i in binary*.pgm;do name=${i##binary_} base=${name%.pgm}; echo ./p2 ${i} p2_${base}.pgm; done;

for i in binary*.pgm;do name=${i##binary_} base=${name%.pgm}; ./p2 ${i} p2_${base}.pgm; done;
```

