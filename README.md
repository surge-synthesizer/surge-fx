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

On linux, and only on linux, if you are a licensee to the Steinberg VST2
distribution and want to build a VST2, you can do the following

```
make -f Makefile.lin super-clean
export VST2SDK_DIR=(location of the VST2 SDK source)
make -f Makefile.lin build
```

and our Makefile will reconfigure jucer to eject a VST2

## Windows

Coming soon.
