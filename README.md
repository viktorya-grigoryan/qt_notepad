Разработать программу, производящую отображение XML-файлов в виде иерархической таблицы.

Основные требования:
1.	xml-Файлы данных брать с Портала открытых данных Правительства Москвы (http://data.mos.ru). 
    Скачать не менее четырех xml-файлов, например, данные по кафе, кофейням, барам и предприятиям быстрого обслуживания.

2.	Программа должна предоставлять меню (`QMenu`), содержащее следующие пункты:
    * Файл:
        + Открыть (загрузка xml-файла данных)
        + Закрыть все (Очистить область вывода данных)
        + Выход (Закрыть программу)

3.	Для открытия файлов использовать стандартное диалоговое окно выбора файлов (`QFileDialog`).
4.	Парсинг xml-файла должен осуществляться в классе модели, унаследованном от `QAbstractItemModel` или производного от него.
    Предоставить конструктор, принимающий имя xml-файла. Для парсинга использовать DOM.
5.	Данные xml-файла должны добавляться в `QTreeView`, элементами которого являются подтаблицы.
6.	Полагать, что структура файла известна и информация в дереве может отличаться от точной структуры xml-файла.
