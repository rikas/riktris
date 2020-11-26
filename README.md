### Fix wav files downloaded from the Internet

```
ffmpeg -i "file.wav" -f wav -bitexact -acodec pcm_s16le -ar 22050 -ac 1 "file_good.wav"
```

### Convert mp3 to ogg

```
ffmpeg -i file.mp3 -c:a libvorbis -q:a 4 file.ogg
```

### Convert wav to ogg

```
ffmpeg -i audio.wav -acodec libvorbis audio.ogg
```

### Bundle in Mac OS X

```
mkdir Riktris.app
mkdir Riktris.app/Content
```

* Create Info.plist

```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleExecutable</key>
	<string>Riktris</string>
	<key>CFBundleIconFile</key>
	<string>Riktris.icns</string>
	<key>CFBundleIdentifier</key>
	<string>com.riktris</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleName</key>
	<string>Riktris</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleShortVersionString</key>
	<string>1.0.0.1</string>
	<key>CFBundleVersion</key>
	<string>1.0.0.1</string>
	<key>LSApplicationCategoryType</key>
	<string>public.app-category.games</string>
	<key>LSMinimumSystemVersion</key>
	<string>10.6</string>
	<key>NSHumanReadableCopyright</key>
	<string>Copyright © 2020 Ricardo Otero. All rights reserved.</string>
</dict>
</plist>
```

* Copy the executable compiled the normal way

```
mv riktris Riktris.app/Contents/MacOS
```

* Copy dependencies with dylibbundler

```
dylibbundler -of -x Riktris.app/Contents/MacOS/riktris -b -d Riktris.app/Contents/MacOS -p @executable_path
```

* Create the icon with this script

```
./create_icns.sh
```
