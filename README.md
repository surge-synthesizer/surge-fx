# Surge-FX Standalone

This is the Surge FX set as a standalone plugin and app.

The software builds and works on mac, linux, and windows. It is currently in an early beta state. If you want
a DAW plugin on linux you need to build it yourself if you are a VST2 licensee. For Mac and Windows, 
[you can get binaries on every push to master](https://github.com/surge-synthesizer/surge-fx/releases). 

There's no manual yet but it is pretty straight forward.

If you find a bug please let us know on our slack or open an issue.

## Building

Since our move to JUCE6, building is simple and the same on all platforms

```
git submodule update --init --recursive
cmake -Bbuild
cmake --build build --config Release
```

this will build the assets in your build directory

```
cmake --build build --config Release --target installer_pkg
```

will build an installer image in build/asset.


## Azure Pipelines

The azure pipelines run the mac and windows builds right now and upload the result.
The main surge Release pipeline also builds the mac fx.
