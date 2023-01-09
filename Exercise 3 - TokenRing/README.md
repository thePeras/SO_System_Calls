
## Exercise 3 - TokenRing

Escreva um programa tokenring que recebe 3 inteiros - ```n```, ```p``` e ```t``` - na linha de comando. Quando executado, cria ```n``` processos ligados entre si por “named pipes”. As “named pipes” devem ter nomes pipe1to2, pipe2to3, ..., pipento1 e, como o nome indica, permitem a comunicação unidirecional entre um processo i e o processo i+1. A última “named pipe” fecha o anel permitindo ao processo n comunicar com o processo 1.

Depois de criado este anel de processos, p1 deverá enviar uma “token” (uma mensagem com um inteiro com valor inicial 0) para o processo seguinte (p1 > p2) e por aí em diante (p2 > p3 > ... > pn > p1 > ...). A “token” deve circular entre os processos ininterruptamente, incrementando o seu valor em cada “hop”. De cada vez que um processo recebe a “token” deve reenviá-la de imediato para o processo seguinte ou, com uma probabilidade de ```p```, bloquear o seu envio durante ```t``` segundos, imprimindo uma mensagem assinalando esse facto (ver o exemplo que se segue). Em ambos os casos, o valor da “token” deve ser incrementado.

```shell
$ tokenring 5 0.01 10
[p2] lock on token (val = 2867)
[p2] unlock token
[p5] lock on token (val = 9213)
[p5] unlock token
...
```