mkdir assets
mkdir products

curl -o assets/juce-5.4.3-windows.zip https://d30pueezughrda.cloudfront.net/juce/juce-5.4.3-windows.zip
cd assets && unzip juce-5.4.3-windows.zip

assets\JUCE\Projucer.exe --resave surge-fx.jucer
cd Builds\VisualStudio2017
msbuild /target:Build surge-fx.sln /p:Configuration=Release;Platform=x64
cd x64\
7z.exe a ..\..\..\products\surge-fx-win.zip Release



