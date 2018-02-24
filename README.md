# beast.c

Clone of classic DOS game in C (SDL)

**Note:** This is a work-in-progress; I haven't gotten very far yet. My [JavaScript](https://github.com/prust/beast.js) and [Lua](https://github.com/prust/beast-lua) implementations are further along.

## SDL Setup for OSX

This game relies on the SDL library. Normally this will be bundled with the game, but if you want to compile & run it from scratch, you'll need to create an SDL library and put it somewhere the compiler can find it:

```
mkdir ~/SDL 
cd ~/SDL 
curl -O http://www.libsdl.org/release/SDL2-2.0.7.zip
tar -xvjpf SDL2-2.0.7.zip
cd SDL2-2.0.7
./configure
make
make install
```

## Compiling (gcc) & Running

```
git clone https://github.com/prust/beast.c.git
cd beast.c
make && ./beast
```
