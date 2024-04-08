# EngineSearch

Download the repository, open cmd (Windows), \
go to the directory where the repository was  \
uploaded and perform the following steps below 

... \repos > cd EngineSearch/build \
... \repos\EngineSearch\build> cmake cmake -DCMAKE_BUILD_TYPE=Debug .. \
... \repos\EngineSearch\build> cmake --build . 

### Next, go to the directory 
... \repos\EngineSearch\build> cd EngineSearch/Debug 
### We enter it in the console 
... \repos\EngineSearch\build\EngineSearch\Debug> EngineSearch.exe 

### example output: 
If all the files are in place that lie in the "resources" and "configs" directories, as well as in the "requests.json" file located in the "configs" directory, you have filled in the "requests" array with requests, then the program will run successfully and the word will appear on the screen "DONE"

In the project directory "configs" in the file "answers.json" a line should appear which is shown below

{"answers":{"request_001":{"relevance":[{"docid":0},{"rank":1},{"docid":2},{"rank":0.857142865},{"docid":3},{"rank":0.571428596},{"docid":1},{"rank":0.571428596}],"result":"true"},"request_002":{"docid":0,"rank":1.0,"result":"true"},"request_003":{"result":"false"}}}

The End!!!