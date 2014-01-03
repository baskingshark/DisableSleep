all:
	xcodebuild -scheme Release -derivedDataPath DerivedData
	xcodebuild -scheme Debug -derivedDataPath DerivedData
clean:
	rm -rf DerivedData
