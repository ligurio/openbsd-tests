## OpenBSD microbenchmarks

### How-To Use

- [Google benchmark](https://github.com/google/benchmark)
- 64-bit: ```make```
- 32-bit: ```make CFLAGS=-m32```
- run `pip install -r requirements.txt`
- execute benchmark

```
$ ./at_2d --benchmark_format=csv | python ../../tools/plot.py
```

Copyright (c) 2017, Sergey Bronnikov sergeyb@bronevichok.ru
