push 0
popr ax

NEXT:
pushr ax
pushr ax
mul
out

pushr ax
push 1
add
popr ax

pushr ax
push 10
jb NEXT

hlt
