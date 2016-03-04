create table game
(
	id int unsigned not null auto_increment primary key,
	requester int not null,
	player int not null,
	stat int not null	
);