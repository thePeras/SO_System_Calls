
## Exercise 1 - Samples

Escreva um programa samples que dado um ficheiro de texto e dois inteiros, ```n``` e ```m```, na linha de comando imprime no terminal um texto constituído por ```n``` fragmentos, cada um com m caracteres, do ficheiro original. Os fragmentos devem ser obtidos em n posições diferentes no ficheiro de forma aleatória e ser escritos sequencialmente no stdout sem qualquer separador ou mudança de linha entre caracteres > e <.

```shell
$ samples
usage: samples file numberfrags maxfragsize

$ cat quote.txt
Look again at that dot. That’s here. That’s home. That’s us. On it everyone
you love, everyone you know, everyone you ever heard of, every human
being who ever was, lived out their lives. The aggregate of our joy and
suffering, thousands of confident religions, ideologies, and economic
doctrines, every hunter and forager, every hero and coward, every creator
and destroyer of civilization, every king and peasant, every young couple
in love, every mother and father, hopeful child, inventor and explorer,
every teacher of morals, every corrupt politician, every "superstar," every
"supreme leader," every saint and sinner in the history of our species
lived there - on a mote of dust suspended in a sunbeam.

$ samples quote.txt 5 7
 >ry king<
 >yone yo<
 >unbeam.<
 >king an<
 >y teach<
```