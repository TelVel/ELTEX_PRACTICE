# 01

Изменить программу 04 так, чтобы дочерний процесс выводил информацию из файла.
Если родительский процесс собирается изменить файл, то он отправляет сигнал SIGUSR1 (блокировка доступа к файлу).
Когда родительский процесс завершил модификацию файла, он отправляет сигнал SIGUSR2 (разрешение доступа к файлу).
Дочерний процесс отправляет новое число родительскому после того, как прочитал файл.

## Execute

To compile run 'make'

To execute run 'callsig' then write amount of numbers that will be generated.
Numbers generated will be in the "shared.txt" file and messages will be stored in "log.txt"

## Clean

To clean executable file run 'make clean'