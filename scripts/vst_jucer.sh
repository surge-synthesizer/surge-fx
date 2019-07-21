#!/bin/bash

if [ -z $VST2SDK_DIR ]; then
   cat surge-fx.jucer
else
   sed -e 's/buildVST3/buildVST,buildVST3/' surge-fx.jucer | \
	sed -e "s@VST2SDK_DIR@${VST2SDK_DIR}@" | \
	sed -e 's/PLUGINHOST_VST="0"/PLUGINHOST_VST="1"/'
fi

