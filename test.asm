push 0
popf ax

NEXT:
pushr ax
pushr ax
mul
out

pushr ax
push 1
add
popf ax

pushr ax
push 10
jb NEXT

hlt

