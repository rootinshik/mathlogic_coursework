# Курсовая работа по математической логике

### Структура проекта
```
.
├── bin
│   └── main <- Исполняемый файл программы
├── docs <- Документация (отчет о проделанной курсовой роботе)
├── include
│   └── buddy <- Заголовки библиотеки Buddy
│       ├── bdd.h
│       ├── bddtree.h
│       ├── bvec.h
│       ├── cache.h
│       ├── config.h
│       ├── fdd.h
│       ├── imatrix.h
│       ├── kernel.h
│       └── prime.h
├── lib
│   └── libBuddy.a <- Собранная из исходников статическая библиотека Buddy
├── README.md
├── results
│   └── solutions.txt <- Результат вычислений
└── src
    └── main.cpp <- Исходный код программы 
```

### Для того чтобы собрать и запустить проект необходимо:

1. Скомпилировать библиотеку Buddy
    1. Скачать библиотеку по [ссылке](https://sourceforge.net/projects/buddy/)
    2. Собрать библиотеку `gcc -c *.c` и `gcc -c *.cxx` 
    3. Упаковать библиотеку `ar rcs libBuddy.a *.o`
    4. Переместить полученный `libBuddy.a` в папку `lib`
2. Собрать проект `g++ -o main src/main.cpp -I./include -L./lib -lBuddy`