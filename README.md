# Traveling Salesman Problem с весами 1, 2

Сам проект по сложностям - project.ipynb

Далее пойдет вспомогательная информация, слабо относящаяся к самому проекту.

## Запуск

* ./compile.sh - скомпилить
* ./generate_graph.sh \<размер графа\> \<seed\> - сгенерировать граф и положить в input.txt. 
Формат: первая строка - количество вершин и ребер. В следующих строчках перечислены ребра неориентированного графа по 1 разу
* ./run.sh - запустить solver на 1 секунду и вернуть вес наилучшего найденного цикла. solver задается в коде main.cpp в переменной this_solver  
* ./test.sh - запустить тесты

## Детали реализации

Чтобы запустить C++ программу на фиксированное время, используются сигналы. Отсюда работает только на Linux. 
Причем из-за сигналов могут появится баги segmentation fault при использовании не signal-safety функций. 

После получения сигнала *SIGTERM* программа подсчитывает вес наилучшего цикла и выводит ответ.

