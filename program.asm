push 10
push 20
push 30
push 40
push 50
pop cx
pop bx
green:
pop ax
push ax
push bx
add
out
jmp green
