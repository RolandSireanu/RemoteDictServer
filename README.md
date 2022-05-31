# Build instructions on Debian based systems:
	1. sudo apt-get install protobuf-compiler
	2. Go To common/ProtoBuffers 
		- mkdir build && cd build
		- cmake ..
		- make all
	3. Do the same for: remoteDictApi, remoteDictServer and clientApp

# Usage instructions:
	1. Start the remote dictionary server:
	2. Link the remoteDictApi library into your project like I did in clientApp example