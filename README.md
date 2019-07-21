# Surge-FX Standalone

This is the Surge FX set as a standalone AU and VST3.

Right now it is mac only and unusable.

But if you are brave 

## Macintosh

```
cd (someplace)
git clone https://github.com/surge-synthesizer/surge-fx
cd surge-fx
git submodule update --init --recursive
make build
```

and you should in theory get a VST3 and AU ready to go.


## Linux

Only difference is the "-f Makefile.lin"

I will fix that one day soon. 

Also linux only builds standalone not VST3 now. I don't quite know why.

```
cd (someplace)
git clone https://github.com/surge-synthesizer/surge-fx
cd surge-fx
git submodule update --init --recursive
make -f Makefile.lin build
```

## Windows

Coming soon.
