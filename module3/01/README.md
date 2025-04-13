# 01

Написать программу, вычисляющую площади квадратов с заданной длиной стороны.
Длины сторон передаются как аргументы запуска.
Расчеты делают родительский и дочерний процессы, разделяя задачи примерно поровну.

## Execute

To compile run 'make'

To execute run 'squares' with any amount of squares as a command line arguments.
For examples './squares 50 40 30 20 10' will return:

Parent of 20: 400
Parent of 10: 100
Child of 50: 2500
Child of 40: 1600
Child of 30: 900

## Clean

To clean executable file run 'make clean'