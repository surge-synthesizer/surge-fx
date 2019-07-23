# Surge-FX Standalone

This is the Surge FX set as a standalone plugin and app.

The software buidls and works mac linux and windows. It is currently in an early beta state. If you want
a DAW plugin on linux you need to build it yourself if you are a VST2 licensee. For Mac and Windows, 
[you can get binaries on every push to master](releases/). 

There's no manual yet but it is pretty straight forward.

If you find a bug please let us know on our slack or open an issue.

Instructions to build below.

## Macintosh

```
cd (someplace)
git clone https://github.com/surge-synthesizer/surge-fx
cd surge-fx
git submodule update --init --recursive
make -f Makefile.mac build
```

and you should in theory get a VST3 and AU ready to go.


## Linux

JUCE doesn't support VST3 so you only get a standalone JACK application.

```
cd (someplace)
git clone https://github.com/surge-synthesizer/surge-fx
cd surge-fx
git submodule update --init --recursive
make -f Makefile.lin build
```

Since JUCE doesn't support VST3, on linux, and only on linux, if you are a 
licensee to the Steinberg VST2 distribution and want to build a VST2, you can do the following

```
make -f Makefile.lin super-clean
export VST2SDK_DIR=(location of the VST2 SDK source)
make -f Makefile.lin build
```

and our Makefile will reconfigure jucer to eject a VST2

## Windows

Windows builds. Because the windows toolchain isn't unixy there's not a nice makefile.

But if you are in shell set up for 64 bit vs2017 then `Make-win.bat` will probably get you there.
If not, look at what it does and do that. 

## Azure Pipelines

The azure pipelines run the mac and windows builds right now and upload the result to a zip.
The main surge Release pipeline also builds the mac fx.
