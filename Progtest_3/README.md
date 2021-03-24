# Úloha 3
## Volby
Úkolem je zajistit politikům během pandemie dostatečnou popularitu s použitím dynamického programování. 
## Hodnocení:	6.0000 / 1.0000

## Formát vstupu
Vstup obsahuje více testovacích dat, na prvním řádku je číslo T, které udává počet vstupů, které musí váš program zpracovat.  
Každý testovací vstup začíná řádkem obsahujícím sedmici čísel:   
**N, A, O, P, F, V a R,**   
kde N je počet týdnů sledovaného období,  
**A** je nejvyšší počet zároveň aktivních opatření,   
**O** je počet možných opatření, které dostaneme na vstupu později,   
**P** je počet voleb, které dostaneme na vstupu později,   
**F** je naše počáteční popularita,   
**V** je počet nakažených na konci prvního týdne,   
**R** je globální rychlost šíření nákazy (desetinné číslo).   
Všechna tyto čísla, kromě **R**, jsou celá čísla.   
Následuje **O** řádků, každý popisující jedno možné pravidlo čtyřmi čísly:   
první tři jsou změna popularity při zavedení / aktivním / zrušení opatření, 
a poslední je desetinné číslo **dR**, tj. číslo, kterým přenásobíme **R**, pokud je ono opatření aktivní.  
Změna popularity je vždy celé číslo.  
Dále následuje P řádků popisující volby: pro každé je první číslo týden voleb, a druhé je minimální požadovaná popularita, abychom je vyhráli.

Vždy platí, že:
T ≤ 100  
N ≤ 100  
A ≤ 5  
O ≤ 6  
0 ≤ P ≤ N-1  
0 ≤ F ≤ 100  
0 ≤ V ≤ 200000  
0 < R < 4  
0 < dR < 2  

## Ukázka práce programu
Ukázkový vstup 1
```
1
8 2 3 1 50 10000 2.0
0 0 0 0.8
0 0 0 0.9
0 -1 2 0.6
7 50
```
Ukázkový výstup 1
```
4 46962
2 zavest 0
3 zavest 2
6 zrusit 2
7 zavest 1
```
