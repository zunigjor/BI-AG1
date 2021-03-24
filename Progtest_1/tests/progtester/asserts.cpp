void assert49 () { // Trim nesmí odstranit počáteční stav automatu
DFA pozor = {
	{ 0 },
	{ 'e', 'l' },
	{},
	0,
	{},
};
assert ( trim(pozor) == pozor ); } 

