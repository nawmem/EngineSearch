# EngineSearch

Download the repository, open cmd (Windows), \
go to the directory where the repository was  \
uploaded and perform the following steps below 

... \repos > cd EngineSearch/build \
... \repos\EngineSearch\build> cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/ninja.exe -DCMAKE_C_COMPILER=C:/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/mingw64/bin/c++.exe -G Ninja .. \
... \repos\EngineSearch\build> cmake --build . 

### Next, go to the directory 
... \repos\EngineSearch\build> cd EngineSearch/Debug 
### We enter it in the console 
... \repos\EngineSearch\build\EngineSearch\Debug> EngineSearch.exe 

Without which the program will not work, read below. \

There should definitely be files in the "configs" directory \
- config.json \
- request. \
sjon answers file.The json will be created automatically so there is no need to create it.

The "resources" directory should contain files with text, and word search will be essential in them. \
They should be named the same as in the "files" field of the "config.json" file \
In the "requests.json" file located in the "configs" directory, fill in the "requests" array with requests for them to be searched.


If all the files are in place and they are filled in, the program will start successfully and the word "DONE" will appear on the screen and the line shown below will appear in the project directory in the "configs" directory in the "answers.json" file. \

### example output: 
{"answers":{"request_001":{"relevance":[{"docid":0},{"rank":1},{"docid":2},{"rank":0.857142865},{"docid":3},{"rank":0.571428596},{"docid":1},{"rank":0.571428596}],"result":"true"},"request_002":{"docid":0,"rank":1.0,"result":"true"},"request_003":{"result":"false"}}}

The End!!!