all:
	xcodebuild -scheme Release install DSTROOT=Build/Release/root
	xcodebuild -scheme Debug   install DSTROOT=Build/Debug/root
clean:
	rm -rf Build
dbgpkg: all
	mkdir -p Build/Debug/root/Library/LaunchDaemons
	cp Package/github.sheeparegreat.DisableSleep.plist Build/Debug/root/Library/LaunchDaemons
	mkdir -p Build/Debug/root/Library/Scripts
	cp -p Package/github.sheeparegreat.DisableSleep.sh Build/Debug/root/Library/Scripts	
	pkgbuild --root Build/Debug/root Build/Debug/DisableSleep.pkg
pkg: all
	mkdir -p Build/Release/root/Library/LaunchDaemons
	cp Package/github.sheeparegreat.DisableSleep.plist Build/Release/root/Library/LaunchDaemons
	mkdir -p Build/Release/root/Library/Scripts
	cp -p Package/github.sheeparegreat.DisableSleep.sh Build/Release/root/Library/Scripts	
	pkgbuild --root Build/Release/root Build/Release/DisableSleep.pkg
