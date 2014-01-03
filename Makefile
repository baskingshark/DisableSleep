all:
	xcodebuild -scheme Release
	xcodebuild -scheme Debug
clean:
	rm -rf DerivedData build
