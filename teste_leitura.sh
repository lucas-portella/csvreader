prog=teste_leitura
dir=TestesRefA2

if [ -f $prog ]; then
	for i in $(ls $dir | grep .csv)
	do
		echo "Teste arquivo $dir/$i"
		./$prog $dir/$i > saida.txt
		diff $dir/$i saida.txt > diff.txt
		if [ $(cat diff.txt | wc -l) -eq 0 ]; then
			echo OK!
		else
			echo ERRO AO TRATAR ARQUIVO
		fi
		
	done
	rm diff.txt 
	rm saida.txt
fi
