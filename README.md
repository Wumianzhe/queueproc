# Инструкции по сборке

1. Клонировать репозиторий https://github.com/Wumianzhe/queueproc.git
2. При сборке с тестами, клонировать модуль doctest. (`git submodule update --init --recursive` в папке проекта)
3. Сборка с помощью `cmake`. *Важно*: в связи с активным использованием [std::format](https://en.cppreference.com/w/cpp/utility/format) минимальная версия компилятора GCC 13 или clang 14.
Для сборки с тестами надо задать переменную cmake `BUILD_TESTS` истинным значением (`1,ON,YES,TRUE,Y` или ненулевое число)
