create table scores
(
	id int unsigned not null auto_increment primary key,
	username char(32) not null,
	scores int unsigned null
);