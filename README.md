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

##example output: 

WORD - a: \
doc_id - 3: count -1 \
WORD - asd_asd1: \
doc_id - 0: count -1 \
WORD - b: \
doc_id - 3: count -1 \
WORD - c: \
doc_id - 3: count -1 \
WORD - dda: \
doc_id - 0: count -2 \
doc_id - 2: count -2 \
WORD - error1: \
doc_id - 1: count -2 \
WORD - error2: \
doc_id - 1: count -1 \
WORD - file: \
doc_id - 2: count -1 \
doc_id - 3: count -3 \
WORD - file001.txts: \
doc_id - 2: count -1 