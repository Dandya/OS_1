;save for write "Danilov Aleksandr KKSO-01-20"
push '0'
push '2'
push '-'
push '1'
push '0'
push '-'
push 'O'
push 'S'
push 'K'
push 'K'
push ' '
push 'r'
push 'd'
push 'n'
push 'a'
push 's'
push 'k'
push 'e'
push 'l'
push 'A'
push ' '
push 'v'
push 'o'
push 'l'
push 'i'
push 'n'
push 'a'
push 'D'

;save for write "Input one symbol: "
push ':'
push 'l'
push 'o'
push 'b'
push 'm'
push 'y'
push 's'
push ' '
push 'e'
push 'n'
push 'o'
push ' '
push 't'
push 'u'
push 'p'
push 'n'
push 'I'

; start with 0 byte of boot
mov ax, 07C0h
mov ds, ax 
; addr of video mem to register es
mov ax, 0b800h
mov es, ax
xor bx, bx

; declaration of one byte for user's symbol
section .bss
	x resb 1

section .text
; start video servis
mov ax, 3
int 10h


; print "Input one symbol: "
mov ecx, 17
; write white letter on black background 
_for_hello:
	pop ax
	mov ah, 0fh
	mov [es:bx], ax
	add bx, 2
	loop _for_hello

; change place of cursor
	mov ah, 02h
	mov dh, 00h
	mov dl, 17
	int 10h
	
; read one symbol
mov ah, 00h
int 16h
; write symbol on screen
mov ah, 0fh
mov [es:bx], ax
add bx, 2
; save symbol
mov [x], al


; go to center
add bx, 1452

; print "Danilov Aleksandr KKSO-01-20"
mov ecx, 28; 28 - symbols
_m1:
	pop ax
	xor ax, [x]
	mov ah, 0fh
	mov [es:bx], ax
	add bx, 2
	loop _m1
jmp $
times 510-($-$$)db 0
dw 0xaa55
times 1024*1024-($-$$) db 0

