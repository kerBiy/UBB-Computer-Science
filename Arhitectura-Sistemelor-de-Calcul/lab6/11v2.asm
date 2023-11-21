bits 32 
global  start 

extern  exit 
import  exit msvcrt.dll

; Se da un sir A de dublucuvinte. Construiti doua siruri de octeti  
;  - B1: contine ca elemente partea superioara a cuvintelor superioare din A
;  - B2: contine ca elemente partea inferioara a cuvintelor inferioare din A
segment data use32 class=data 
    sir  dw  10abcb0h, a051ad2h, 74120ca1h
	len equ ($ - sir) / 4 ; lungimea sirului
    b1 times len db 0 ; 01, 0a, 74
    b2 times len db 0 ; b0, d2, a1

segment code use32 class=code 
    start: 
        mov esi, sir
        mov edi, b1
        cdf ; pargcurgem de la st la dr
        mov ecx, len ; punem lungimea in ecx
        mov edx, 0 ; iterator pe b2 
        
        jecxz Sfarsit ; metoda anti-prost

        Repeta: 
            lodsd ; in eax vom avea elementul curent

            mov [b2 + edx], al ; partea inf a cuv inf
            inc edx ; incrementare pt b2

            shr eax, 12 ; rotim pana la partea sup din cuv sup
            stosb ; in edi punem pt sup a cuv sup la poz curenta
        loop Repeta

    Sfarsit:

        push dword 0
        call [exit] 