#!/bin/bash

exe=teste
dir_arq=TestesRefA2
saida=copia.csv

if [ -f $exe ]
then
	for i in $(ls $dir_arq)
	do	
		echo Teste arquivo: $dir_arq/$i
		./$exe $dir_arq/$i > $saida
		diff $saida $dir_arq/$i > dif.txt
		if [ $(wc -l dif.txt) -eq 0 ]
		then
			echo '..... OK'
		else
			echo '..... ERRO DE LEITURA'
		fi
		rm $saida
		rm dif.txt
	done
fi
