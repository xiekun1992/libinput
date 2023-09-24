mkdir -p ./build/
cd ./build
rm -rf ./Debug/
cmake -B . -S ..
cmake --build .

cp -f ./Debug/libcapture.dll ./Debug/libcapture.lib /e/Desktop/hello/