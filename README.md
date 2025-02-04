# Курсовая работа по математической логике

### Структура проекта
```
.
.
├── bin
│   └── main <- Главный исполняемый файл
├── docs <- Документы (Отчеты о курсовой)
│   ├── ...
├── include
│   ├── buddy <- Заголовочные файлы библиотеки Buddy
│   │   └── ... 
│   ├── config.h <- Инициализация для решения задачи
│   └── ... <- Прочие заголовочные файлы для *.cpp из src
├── lib
│   └── libBuddy.a <- Собранная библиотека статическая бибилиотека Buddy
├── README.md 
├── results
│   └── solutions.txt <- Файл с итоговыми решениями
└── src
    ├── bdd_manager.cpp <-  
    ├── constraints.cpp <- Ограничения для задачи Энштейна
    ├── main.cpp <- Главная часть программы
    └── output.cpp <- Функции для вывода решения (в файл и в консоль)
```

### Для того чтобы собрать и запустить проект необходимо:

1. Скомпилировать библиотеку Buddy
    1. Скачать библиотеку по [ссылке](https://sourceforge.net/projects/buddy/)
    2. Собрать библиотеку `gcc -c *.c` и `gcc -c *.cxx` 
    3. Упаковать библиотеку `ar rcs libBuddy.a *.o`
    4. Переместить полученный `libBuddy.a` в папку `lib`
2. Собрать проект: 
```
g++ -o bin/main \
                                      src/main.cpp \
                                      src/bdd_manager.cpp \
                                      src/constraints.cpp \
                                      src/output.cpp \
                                      -I./include \
                                      -L./lib -lBuddy \
                                      -std=c++17 \
                                      -Wall -Wextra -Werror
```
