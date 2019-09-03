rem mkdir assets


rem curl -o assets/juce-5.4.3-windows.zip https://d30pueezughrda.cloudfront.net/juce/juce-5.4.3-windows.zip
rem cd assets && unzip juce-5.4.3-windows.zip
rem cd ..

assets\JUCE\Projucer.exe --resave surge-fx.jucer
cd Builds\VisualStudio2017
msbuild /target:Build SurgeEffectsBank.sln /p:Configuration=Release;Platform=x64
msbuild /target:Build SurgeEffectsBank.sln /p:Configuration=ReleaseWin32;Platform=Win32




