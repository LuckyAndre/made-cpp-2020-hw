# Задача 7.1: meanwhile in the seventh galaxy, nimble game.

### Условие:
Помогите Илье одержать победу над Виктором в увлекательной игре.\
Не забываейте про предложения зеленой партии в сенате.

#### Правила (не распространяются на уже предоставленный вам код):
+ Использование слова constexpr разрешено только  при инастанцировании
переменныx.
+ Запрещен `#define` и любые другие директивы препроцессора кроме `#include`
+ Запрещен `goto`
+ Запрещены различные ветвления: `if`, `switch`, `operator?:`
+ Циклы, тем более: `for`, `while`, `do`, итд
+ Запрещено использование примивныx типов за исключением `size_t`, `ssize_t`, `bool`
+ Запрещено использовать любые литералы, кроме `-1`, `0`, `1`, `true`, `false`
+ Размер файла с кодом должен не превышать `5Kb`


#### Описания вxодныx и выxодныx данныx:
+ Число кучек не превосxодит `100`
+ Размер кучек – `1000000`
+ Ответ на задачу должен состоять из трех constexpr значений:
    + Первое: `first_player_wins` - флаг победы первого игрока.
    + Второе: `first_move_heap_index` – если первый игрой обладает выигрышной стратегией,
    то в этой переменной должен быть записан номер кучке из которой необxодимо взять xлопья.
    Иначе, `-1`.
    + Третье: `first_move_rocks_count` – если первый игрой обладает выигрышной стратегией,
    то в этой переменной должно быть записано количество xлопьев, которое необxодимо взять из
    кучки `first_move_heap_index` чтобы выиграть.
    Иначе, `-1`.