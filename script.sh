#!/bin/bash

exec=teste
dir_arq=TestesRefA2


if [-f $(exec) ]
then
	for (i in $(ls $(dir_arq)))
	do	
		./$(exec) $(dir_arq)/$i > saida
		diff saida $(dir_arq)/$i > erros.txt
	done
fi
