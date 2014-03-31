ldif-gen
========

ldif-gen: A Bulk LDIF Generator (in C)

##Simple example, generates 10,000 users
~~~
$ bash build.sh
...
$ ./bin/ldif-gen.O3-64 >/tmp/test.ldif
~~~

##10,000 users with photos
~~~
$ ln -s ${PWD}/faces/ /tmp/
...
$ ./bin/ldif-gen.O3-64 >/tmp/test.ldif
~~~


##1,000,000 users
~~~
$ ./bin/ldif-gen.O3-64 -i 1000000  >/tmp/test.ldif
~~~