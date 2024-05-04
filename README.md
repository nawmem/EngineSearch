# EngineSearch


### **Как собрать и запустить проект.**

Для начала склонируйте проект из репозитория, рядом с другими проектами
>  **<font color="#e07979">Обязательно проверьте:</font>**  
> 
> Наличие директории "nlohmann_json" в корне проекта т.к. используется библиотека nlohmann_json для работы с json файлами
> 
> А так-же в директории **"...\repos\EngineSearch\configs"** должны быть 2 файла
> - configs.json
> - requests.json
> - answers.json создается автоматически

#### Пример config.json
<pre>
{
    "config": {
        "name": "SearchEngine",     <-- Название поекта
        "version": "0.1",           <-- Версия поекта
        "max_response": 5           <-- Максимальное колличествозапросов
    },
    "files": [
        "resources/file001.txt",    <-- Пути к файлам в которых
        "resources/file002.txt",    <-- будет произведен поиск
        "resources/file003.txt",    <-- слов
        "resources/file004.txt"
    ]
}
</pre>

####  Пример config.json
<pre>
{
  "requests": [
    "The words that will be searched in the files",
    "The words that will be searched in the files",
    "The words that will be searched in the files"
  ]
}
</pre>

### **Собираем проект**

> **<font color="a1a852">Кофигурация:</font>**  \
> Система сборки - ninja \
> Процессом сборки руководит - cmake (v-3.25) \
> Компилятор - mingw64

>Проект также <font color="green">успешно</font> запускался и тестировался на компиляторе **MSVC 2022**, и **CMake 3.9**

продолжим ... 

<pre>
Перейдите в корневую директорию проекта
...\repos > cd EngineSearch

Создайте в ней папку "build"
...\repos\EngineSearch > mkdir build

Перейдите в нее
...\repos\EngineSearch > cd build
</pre>

**Создаем финальный скрипт сборки**

Пример
 <pre>
...\repos\EngineSearch\build> cmake -DCMAKE_BUILD_TYPE=Debug <--- С отладкой или релиз
путь к системе сбоки     ----------> -DCMAKE_MAKE_PROGRAM="C:/ninja.exe"
путь к С компилятору     ----------> -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe"
путь к С++ компилятору   ----------> -DCMAKE_CXX_COMPILER="C:/mingw64/bin/c++.exe" 
указываем систему сборки ----------> -G Ninja 
путь к директории исходных файлов -> ..
 </pre>
Если все прошло успешно на консоли появится
<pre>
...\EngineSearch\build>cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/ninja.exe -DCMAKE_C_COMPILER=C:/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/mingw64/bin/c++.exe -G Ninja ..
-- The CXX compiler identification is GNU 13.2.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/mingw64/bin/c++.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Using the multi-header code from .../EngineSearch/nlohmann_json/include/
-- Could NOT find Python (missing: Python_EXECUTABLE Interpreter)
-- Configuring done (9.2s)
-- Generating done (0.0s)
-- Build files have been written to: .../EngineSearch/build
</pre>

### **Компилируем проект**
`...\repos\EngineSearch\build> cmake --build . `

Если все прошло успешно то в консоли вы увидите  \
<pre>
...\repos\EngineSearch\build>cmake --build .
[20/20] Linking CXX executable gtest\EngineSearchTest.exe
</pre>

> **<font color="#a8534d">Важно:</font>** \
Если будут ошибки после комманды `cmake --build .`  \
Нужно взять файл <font color="#7681b8">libgcc_s_seh-1.dll</font> и  <font color="#7681b8">libstdc++-6.dll</font> из директории с компилятором **...\mingw64\bin** и положить их рядом с .exe файлами


Эти файлы нужно скопировать в папки  \
**...\repos\EngineSearch\build**  \
**...\EngineSearch\build\gtest**

После этого еще раз ввести команду `cmake --build .`
### **Запускаем проект**  
Находим файл **EngineSearch.exe**
Открываем его в **консоли**  \
`...\repos\EngineSearch\build> EngineSearch.exe `

> Если в каталоге "configs" нет файлов **"config"** или **"requests"**, программа сообщит об этом выводом **"ATTENTION:"** в консоли. \
> **Пример:**
> <pre>
> ...\repos\EngineSearch\build\gtest>EngineSearchTest.exe
> ATTENTION: There is no such file "requests.json" at "...\EngineSearch\configs" 
> </pre>

В итоге в консоль выйдет слово "DONE" - это ознаечает, что программа выполнена успешно

### Пример вывода
<pre>
...\repos\EngineSearch\build\EngineSearch.exe
DONE
</pre>

#### Файл answers.json
    {
    "answers":{
        "request_001":{
            "relevance":[
                {"docid":0},{"rank":1},
                {"docid":2},{"rank":0.857142865},
                {"docid":3},{"rank":0.571428596},
                {"docid":1},{"rank":0.571428596}
            ],
            "result":"true"
        },
        "request_002":{
            "docid":0,"rank":1.0,
            "result":"true"
        },
            "request_003":{
                "result":"false"
            }
        }
    }

### **Запускаем тесты** 

Переходим в директорию **gtest**  \
`...\repos\EngineSearch\build>cd gtest`

Запускаем  \
`...\repos\EngineSearch\build\gtest>EngineSearchTest.exe`

#### Пример успешных тестов
<pre>
...\repos\EngineSearch\build\gtest>EngineSearchTest.exe
Running main() from .../repos/EngineSearch/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 5 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from TestCaseInvertedIndex
[ RUN      ] TestCaseInvertedIndex.TestBasic
[       OK ] TestCaseInvertedIndex.TestBasic (0 ms)
[ RUN      ] TestCaseInvertedIndex.TestBasic2
[       OK ] TestCaseInvertedIndex.TestBasic2 (0 ms)
[ RUN      ] TestCaseInvertedIndex.TestInvertedIndexMissingWord
[       OK ] TestCaseInvertedIndex.TestInvertedIndexMissingWord (0 ms)
[----------] 3 tests from TestCaseInvertedIndex (14 ms total)

[----------] 2 tests from TestCaseSearchServer
[ RUN      ] TestCaseSearchServer.TestSimple
[       OK ] TestCaseSearchServer.TestSimple (0 ms)
[ RUN      ] TestCaseSearchServer.TestTop5
[       OK ] TestCaseSearchServer.TestTop5 (2 ms)
[----------] 2 tests from TestCaseSearchServer (9 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 2 test suites ran. (41 ms total)
[  PASSED  ] 5 tests.
</pre>

если из какойто из тестов будет провален вы увидите пример ниже

<pre>
...\repos\EngineSearch\build\gtest>EngineSearchTest.exe
Running main() from .../repos/EngineSearch/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 5 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from TestCaseInvertedIndex
[ RUN      ] TestCaseInvertedIndex.TestBasic
.../repos/EngineSearch/gtest/MainTest.cpp:21: Failure
Expected equality of these values:
  result
    Which is: { { 16-byte object <00-00 00-00 00-00 00-00 01-00 00-00 00-00 00-00> }, { 16-byte object <00-00 00-00 00-00 00-00 01-00 00-00 00-00 00-00>, 16-byte object <01-00 00-00 00-00 00-00 03-00 00-00 00-00 00-00> } }
  expected
    Which is: { { 16-byte object <00-00 00-00 00-00 00-00 02-00 00-00 00-00 00-00> }, { 16-byte object <00-00 00-00 00-00 00-00 01-00 00-00 00-00 00-00>, 16-byte object <01-00 00-00 00-00 00-00 03-00 00-00 00-00 00-00> } }
[  FAILED  ] TestCaseInvertedIndex.TestBasic (21 ms)
[ RUN      ] TestCaseInvertedIndex.TestBasic2
[       OK ] TestCaseInvertedIndex.TestBasic2 (0 ms)
[ RUN      ] TestCaseInvertedIndex.TestInvertedIndexMissingWord
[       OK ] TestCaseInvertedIndex.TestInvertedIndexMissingWord (0 ms)
[----------] 3 tests from TestCaseInvertedIndex (34 ms total)

[----------] 2 tests from TestCaseSearchServer
[ RUN      ] TestCaseSearchServer.TestSimple
[       OK ] TestCaseSearchServer.TestSimple (0 ms)
[ RUN      ] TestCaseSearchServer.TestTop5
[       OK ] TestCaseSearchServer.TestTop5 (2 ms)
[----------] 2 tests from TestCaseSearchServer (9 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 2 test suites ran. (63 ms total)
[  PASSED  ] 4 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] TestCaseInvertedIndex.TestBasic

 1 FAILED TEST
</pre>

