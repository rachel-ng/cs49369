
```bash 
for i in PgmImages/*.pgm; do echo ./p1 ${i} 120  binary_${i##*/}; done;
```

```bash 
for i in PgmImages/*.pgm; do ./p1 ${i} 120  binary_${i##*/}; done;
```


