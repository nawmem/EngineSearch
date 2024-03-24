# EngineSearch


... \repos > cd EngineSearch/build
... \repos\EngineSearch\build> cmake cmake -DCMAKE_BUILD_TYPE=Debug ..
... \repos\EngineSearch\build> cmake --build .

Далее в переходим в директорию 
... \repos\EngineSearch\build> cd EngineSearch/Debug
Вводим консоли
... \repos\EngineSearch\build\EngineSearch\Debug> EngineSearch.exe

##пример вывода:

asd_asd1 dda dda
file a file b file c
error1 error2 error1
dda file dda file001.txts
0 - 2
2 - 2
-----------------
WORD - a:
doc_id - 3: count -1
WORD - asd_asd1:
doc_id - 0: count -1
WORD - b:
doc_id - 3: count -1
WORD - c:
doc_id - 3: count -1
WORD - dda:
doc_id - 0: count -2
doc_id - 2: count -2
WORD - error1:
doc_id - 1: count -2
WORD - error2:
doc_id - 1: count -1
WORD - file:
doc_id - 2: count -1
doc_id - 3: count -3
WORD - file001.txts:
doc_id - 2: count -1
