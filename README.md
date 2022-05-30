# Build client library
cmake -DBuildRemoteDictLib=On -DGenerateProto=On -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
make all -j
sudo make install 


# Build server
cmake -DBuildServer=On -DGenerateProto=On ..
make all -j

