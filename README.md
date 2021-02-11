ldif-gen
========

ldif-gen: A Bulk LDIF Generator (in C)

## Simple example, generates 10,000 users
```shell
$ bash build.sh
...
$ ./bin/ldif-gen.O3-64 >/tmp/test.ldif
```

## 10,000 users with photos
```shell
$ ln -s ${PWD}/faces/ /tmp/
...
$ ./bin/ldif-gen.O3-64 >/tmp/test.ldif
```


## 1,000,000 users
```shell
$ ./bin/ldif-gen.O3-64 -i 1000000  >/tmp/test.ldif
```
