# Зайцева Юлия Александровна, БПИ213

# Домашнее задание №4
Разработка многопоточного приложения.

## Вариант задания 25: 
Задача о производстве булавок. В цехе по заточке булавок все необходимые операции осуществляются тремя рабочими. Первый из них берет булавку и проверяет ее на предмет кривизны. Если булавка не кривая, то рабочий передает ее своему напарнику. Напарник осуществляет собственно заточку и передает заточенную булавку третьему рабочему, который осуществляет контроль качества операции. Требуется создать многопоточное приложение, моделирующее работу цеха. При решении использовать парадигму «производитель-потребитель». Следует учесть, что каждая из операций выполняется за случайное время которое не связано с конкретным рабочим. Возможны различные способы передачи (на выбор). Либо непосредственно по одной булавке, либо через ящики, в которых буферизируется некоторое конечное количество булавок.


## Реализация программы на 9
1. **Парадигма «производитель-потребитель»** 

Производители и потребители – это парадигма взаимодействующих неравноправных потоков. Процесс-производитель выполняет вычисления и выводит поток результатов. Процесс-потребитель вводит поток значений и анализирует его. Многие программы в той или иной степени являются производителями и/или потребителями. 

Интересен случай, когда производители и потребители объединены в конвейер - последовательность процессов, в которой каждый потребляет данные предшественника и поставляет данные для последующего процесса.

Между производителем и потребителем существует однонаправленный поток информации. Такой вид межпроцессного взаимодействия не имеет аналогов в последовательном программировании, поскольку в последовательной программе только один поток управления, в то время как производители и потребители - независимые процессы со своими потоками управления и собственными скоростями выполнения.

Другим способом организации потоков потребителей-производителей являются различные древовидные структуры. Самая распространенная из них - организация процесса вычислений методом сдваивания (дихотомия). С помощью этого мощного подхода удобно распараллеливать алгоритмы нахождения сумм элементов массива, его максимального элемента и других групповых функций от массивов, а также вычисления по различным рекуррентным формулам.

Источник: https://studref.com/702392/informatika/paradigmy_parallelnyh_prilozheniy

В данной задаче 2 производителя и 2 потребителя: 1 поток (проверяющий булавки на кривизну) - производитель, 2 поток (осуществляющий заточку) - производитель и потребитель одновременно, 3 поток (проверяющий заточку) - потребитель. В начале программы вводится из файла (или генерируется) количество булавок, которые будут опработаны "конвейером".

2. **В программе на Си реализовано:**

a. 3 потока. Создаем 3 семафоры, чтобы создать зависимость между потоками. Для правильной работы первого потока используется симафор 3. Первый поток проверяет булавки на кривизну. Если булавка соответствует требованиям, то передает ее 2 потоку, увеличивая значение семафора 1 через sem_post, а если нет, тогда начинает проверять другую булавку, увеличивая значение семафора 3. Далее 2 поток осуществяет заточку и увеличивает значение семафора 2. После заточки передает булавку 3 потоку, который проверяет результат операции, хранящийся в переменной sharpness. Также 2 и 3 поток взаимосвязаны через переменную sharpness. Если она равна 2, то это значит, что 3 поток закончил проверять результат операции 2 потока, и можно передать ему булавку.

b. Генерация данных при помощи опции gen в диапазоне от 5 до 14. Сгенерированное значение выводится на экран;

c. Получение данных из файла при помощи опции file;

d. Получение данных из консоли;

e. Рандомизация времени выполнения каждого потока;

f. Использование семафор для смены потоков;

g. Результаты работы программы выводятся одновременно и на экран, и в файл;

h. В консоль выводятся все события на доступном языке. Выводятся последовательно цикл, значение curvness (кривизна), значение sharpness (заточка) и выполняемый поток.

3. **Тестовое покрытие:** 

Результат в программе на Си:

1. При работе с файлами:

Ввод

![ФАйл](https://user-images.githubusercontent.com/97798186/206793764-f6cda0fd-1da2-4099-b346-962dcea1d6a7.jpg)

Вывод

![ФАйл](https://user-images.githubusercontent.com/97798186/206793809-b855c606-ff79-4dd8-8b2c-130595d2c26a.jpg)

2. При генерации данных:

![ФАйл](https://user-images.githubusercontent.com/97798186/206794712-4b2ca86d-eb91-4397-922f-e77cb9d499ea.jpg)

5. **Сравнение на производительность:** 

a. Для сравнения был использован файл, в котором a = 10, b = 20, нижний предел = 2, верхний предел = 13. Была использована функция clock().

Сначала была протестирована программа на Си. Ее результат:

![ФАйл](https://user-images.githubusercontent.com/97798186/202865151-d22e6ee0-d02a-4d4e-acf9-6c3b54b9ce53.jpg)

Затем протестировали программу на ассемблере:

![ФАйл](https://user-images.githubusercontent.com/97798186/202905503-3c61cf60-9e5e-4024-bb99-2baa5ef99ae5.jpg)

Вывод: программа на ассемблере с рефакторингом работает в 5 раз дольше программы на Си.

6. **Сравнение по ассемблерных программ с опциями оптимизации:** 

a. Использование опций оптимизаций по скорости. Были использованы следующие оптимизации: -O3 и -pipe (файл "assemble скорость.s")

b. Использование опций оптимизаций по размеру. Были использованы такие оптимизации: -Os, -fomit-frame-pointer (файл "assemble размер.s")

c. Также были изучены другие опции: -O1, O2, Ofast.

Результаты тестирований (без учета времени ввода и вывода данных) представлены в таблице:

![ФАйл](https://user-images.githubusercontent.com/97798186/202906666-c67c15ac-8aaa-4b5f-aaba-ecb51a18f040.jpg)

Программа без рефакторингом и с рефакторингом работают дольше всего, причем у программы с рефакторингом производительность самая низкая. Программа с оптимизациями по скорости оказалась не самой быстрой: самой производительно оказалась программа с опцией -О1. Как и ожидалось, программа с оптимизациями по размеру имеет самый небольшой объем ассемлерного кода.
