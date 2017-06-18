section .text
        global multiplicar
multiplicar:
        mov RAX, RDI
        mov RCX, RSI
        imul RCX
        ret
