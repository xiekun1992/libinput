mkdir -p ./build/
cd ./build
rm -rf ./Debug/
cmake -B . -S ..
cmake --build .

cp -f ./Debug/libinput.dll ./Debug/libinput.lib /e/Desktop/hello/
cp -f ./Debug/libinput.dll ./Debug/libinput.lib /e/Desktop/lctrl_rust/