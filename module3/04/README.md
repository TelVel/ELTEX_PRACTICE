# 04

Написать программу, порождающую дочерний процесс и использующую однонаправленный обмен данными.
Процесс-потомок генерирует случайные числа и отправляет родителю.
Родитель выводит числа на экран и в файл.
Количество чисел задается в параметрах запуска приложения.

## Execute

To compile run 'make'

To execute run 'call' then write amount of numbers that will be generated.

## Examples

IN "./call 3"
OUT "818 362 425"

IN "./call 3"
OUT "398 12 693"

## Clean

To clean executable file run 'make clean'