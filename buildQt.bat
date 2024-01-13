cmake -S . -B build "-GVisual Studio 16 2019" "-DCMAKE_PREFIX_PATH:STRING=D:\Qt\5.15.2\msvc2019_64"

cmake --build build/ --target ALL_BUILD --config Release