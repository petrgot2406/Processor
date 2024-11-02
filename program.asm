push 1
pop ax

NEXT:
push ax
push ax
mul
out

push ax
push 1
add
pop ax

push ax
push 100
jbe NEXT

hlt
