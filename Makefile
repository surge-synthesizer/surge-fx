# This makefile just assembles assets and runs jucer and stuff.
# And right now is macos only

all: Builds/MacOSX/build/Release/libsurge-fx.a

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

