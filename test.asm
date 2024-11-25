push 0
popf ax

NEXT:
pushf ax
pushf ax
mul
out

pushf ax
push 1
add
popf ax

pushf ax
push 10
jb NEXT

hlt

