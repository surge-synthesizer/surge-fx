# This makefile just assembles assets and runs jucer and stuff.
# And right now is macos only

SURGE_FX_VERSION=$(shell git rev-parse --short HEAD)
BUILDDATE=$(shell date +%Y%m%d)

all: build

assets:
	mkdir -p assets

assets/juce-5.4.3-osx.zip:
	curl -o assets/juce-5.4.3-osx.zip https://d30pueezughrda.cloudfront.net/juce/juce-5.4.3-osx.zip
	cd assets && unzip juce-5.4.3-osx.zip

assets/JUCE: assets assets/juce-5.4.3-osx.zip

Builds/MacOSX/surge-fx.xcodeproj/projects.pbxproj:	assets/JUCE surge-fx.jucer
	assets/JUCE/Projucer.app/Contents/MacOS/Projucer --resave surge-fx.jucer
	touch $@

Builds/MacOSX/build/Release/libsurge-fx.a: Builds/MacOSX/surge-fx.xcodeproj/projects.pbxproj
	xcodebuild build -configuration Release -project Builds/MacOSX/surge-fx.xcodeproj

build:	Builds/MacOSX/surge-fx.xcodeproj/projects.pbxproj 
	xcodebuild build -configuration Release -project Builds/MacOSX/surge-fx.xcodeproj

build-au:	Builds/MacOSX/surge-fx.xcodeproj/projects.pbxproj 
	xcodebuild build -target "surge-fx - AU" -configuration Release -project Builds/MacOSX/surge-fx.xcodeproj

validate:	build-au
	auval -vt aufx VmbA

validate-dbg:	build-au
	lldb -- auval -vt aufx VmbA

clean:
	xcodebuild clean -configuration Release -project Builds/MacOSX/surge-fx.xcodeproj

super-clean:
	rm -rf Builds JuceLibraryCode assets

dmg: build
	rm -rf Builds/MacOSX/surge-fx-nightly
	rm -f *dmg
	mkdir -p Builds/MacOSX/surge-fx-nightly	
	cd Builds/MacOSX/build/Release && tar cf - surge-fx.component/* | ( cd ../../surge-fx-nightly ; tar xf - )
	cd Builds/MacOSX/build/Release && tar cf - surge-fx.vst3/* | ( cd ../../surge-fx-nightly ; tar xf - )
	cd Builds/MacOSX/ && hdiutil create /tmp/tmp.dmg -ov -volname "SurgeFX_${SURGE_FX_VERSION}_${BUILDDATE}" -fs HFS+ -srcfolder ./surge-fx-nightly/
	hdiutil convert /tmp/tmp.dmg -format UDZO -o SurgeFX_${SURGE_FX_VERSION}_${BUILDDATE}.dmg

