push 1
popf ax

NEXT:
pushf ax
pushf ax
mul
pushf ax
mul
out

pushf ax
push 1
add
popf ax

pushf ax
push 1000
jbe NEXT

hlt
