# This makefile just assembles assets and runs jucer and stuff.
# And right now is macos only

all: Builds/MacOSX/build/Release/libsurge-fx.a

assets:
	mkdir -p assets

assets/JUCE: assets
	curl -o assets/juce-5.4.3-osx.zip https://d30pueezughrda.cloudfront.net/juce/juce-5.4.3-osx.zip
	cd assets && unzip juce-5.4.3-osx.zip

Builds/MacOSX/surge-fx.xcodeproj:	assets/JUCE surge-fx.jucer
	assets/JUCE/Projucer.app/Contents/MacOS/Projucer --resave surge-fx.jucer

Builds/MacOSX/build/Release/libsurge-fx.a: Builds/MacOSX/surge-fx.xcodeproj
	xcodebuild build -configuration Release -project Builds/MacOSX/surge-fx.xcodeproj

clean:
	xcodebuild clean -configuration Release -project Builds/MacOSX/surge-fx.xcodeproj

super-clean:
	rm -rf Builds JuceLibraryCode assets

