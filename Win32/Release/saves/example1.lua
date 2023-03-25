first_point = 0
last_point = 3
amount = 20
cycles_amount = 10
time_ratio = 1
price_ratio = 1
score_ratio = 1

map = {
	{
		time = 0.51,
		price = 0,
		score = 3.45,
		name = "Заповедник Ягорлык",
		x = 311,
		y = 67,
		connections = { 0, 5.02, 9.3, 16.82, 8.37, 0, }
	},
	{
		time = 19.59,
		price = 0,
		score = 3.86,
		name = "Дом музей в селе Роги",
		x = 177,
		y = 407,
		connections = { 5.02, 0, 19.19, 14.79, 11.13, 1.97, }
	},
	{
		time = 5.45,
		price = 0,
		score = 0.04,
		name = "Восхождение к водопаду",
		x = 221,
		y = 395,
		connections = { 9.3, 19.19, 0, 20.74, 9.79, 11.02, }
	},
	{
		time = 19.15,
		price = 0,
		score = 7.76,
		name = "Дубоссарский зоопарк",
		x = 167,
		y = 634,
		connections = { 16.82, 14.79, 20.74, 0, 8.33, 1.9, }
	},
	{
		time = 4.6,
		price = 0,
		score = 5.2,
		name = "Дубоссарская ГЭС",
		x = 61,
		y = 697,
		connections = { 8.37, 11.13, 9.79, 8.33, 0, 11.11, }
	},
	{
		time = 10.61,
		price = 0,
		score = 7.77,
		name = "Озеро с лебедями и утками",
		x = 300,
		y = 758,
		connections = { 0, 1.97, 11.02, 1.9, 11.11, 0, }
	},
}

connections = {
	{ id1 = 5, id2 = 0, time = 0 },
	{ id1 = 2, id2 = 0, time = 9.3 },
	{ id1 = 1, id2 = 0, time = 5.02 },
	{ id1 = 4, id2 = 0, time = 8.37 },
	{ id1 = 4, id2 = 1, time = 11.13 },
	{ id1 = 3, id2 = 0, time = 16.82 },
	{ id1 = 1, id2 = 2, time = 19.19 },
	{ id1 = 3, id2 = 2, time = 20.74 },
	{ id1 = 5, id2 = 2, time = 11.02 },
	{ id1 = 4, id2 = 2, time = 9.79 },
	{ id1 = 3, id2 = 1, time = 14.79 },
	{ id1 = 5, id2 = 1, time = 1.97 },
	{ id1 = 4, id2 = 3, time = 8.33 },
	{ id1 = 5, id2 = 3, time = 1.9 },
	{ id1 = 5, id2 = 4, time = 11.11 },
}