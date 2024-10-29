push 10
push 20
push 30
push 40
push 50
red:
pop cx
pop bx
green:
pop ax
push ax
blue:
push bx
add
out
jmp green
