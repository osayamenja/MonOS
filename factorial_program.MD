```asm
/*compute b = a! % |d|*/

load a, 5
load b, 1
load c, -1
load d, -100
load temp, 0

main:
      dump registers
      dump memory
      // Note that instructions below implicitly address the AC register
      factorial:
                  load b // AC = b
                  mul a // AC *= a;
                  // decrement b*a by |d| till it's 0 or negative.
                  mod:
                           store temp // temp = AC
                           add d // AC += d
                           bgz mod // if AC > 0 goto mod
                           mul c // AC *= -1
                           bgz update
                           store temp
                  update:
                           load temp
                  store b
                  print // print AC
                  load a
                  add c
                  store a
                  print
                  dump registers
                  dump memory
                  bgz factorial // if AC > 0 go to factorial
      dump registers
      dump memory
      load b
      print  // print final result
      sleep 20000
      exit // 0 0
```