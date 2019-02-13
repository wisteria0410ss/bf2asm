# これはなに？

Brainf**kのコードを読み込んでアセンブラを吐きだします。

# つかいかた

1. `bfc.c`をコンパイルします
2. コマンドライン引数にBrainf**kのファイル名を指定して実行します
3. 標準出力に対応するアセンブラが出力されます

つまり、
```
gcc bfc.c -o bfc
./bfc hello.bf > hello.asm
nasm -felf64 hello.asm -o hello.o
ld hello.o -o a.out
./a.out
```
で`hello.bf`をコンパイルして実行できます。

Linuxの64 bitマシンを想定しています。また、`nasm`が必要です。