# Brainf**k2nasm

Brainf**kのコードを読み込んでアセンブラを吐きだします。
```
gcc bfc.c -o bfc
./bfc hello.bf > hello.asm
nasm -felf64 hello.asm
ld hello.o
./a.out
```
で`hello.bf`をコンパイルして実行できます。