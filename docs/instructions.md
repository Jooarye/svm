# Instructions
```
lda <x>   -> load the x-th argument 
ldc <x>   -> load x as a constant

cmplt     -> is s-1 < s where s is the addrs on the stack
cmpgt     -> is s-1 > s
cmplq     -> is s-1 <= s
cmpgq     -> is s-1 >= s
cmpeq     -> is s-1 == s
cmpne     -> is s-1 != s

jmpz <a>  -> set ip = a if s == 0
jmpnz <a> -> set ip = a if s != 0

add       -> adds the top two elements on the stack
sub       -> subtracts s from s-1
mul       -> multiplies the top two elements on the stack
div       -> divides s-1 by s

dup       -> duplicates the top element
drop      -> drop the top element
get <x>   -> get the xth element from the top of the stack

call <a>  -> call function at address a
ret <x>   -> return one arg and cleanup x arguments

hlt       -> halt aka. end
```