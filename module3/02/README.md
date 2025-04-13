# 02

Написать программу, похожую на командный интерпретатор.
При запуске выводится приглашение, и пользователь вводит имя и аргументы программы, которую желает запустить.
Программы для запуска написать самостоятельно и поместить в тот же каталог (например, программа для вычисления суммы аргументов, «склеивания» строк, поиска наибольшего значения или наибольшей длины строки и т.д.)

## Execute

To compile run 'make'

To execute run 'interpret' and then write one of the four commands: "sum", "maxnum", "maxstr", "strplus".
After that you write amount of arguments and then arguments themselves.

### sum
returns sum of the arguments.
### maxnum
returns maximum number from the arguments
### maxstr
returns longest string from the arguments
### strplus
returns strings combined into one

## Example
IN: "./interpret 
    > sum
    3
    20 40 60"
OUT
    "120"

## Clean

To clean executable file run 'make clean'
