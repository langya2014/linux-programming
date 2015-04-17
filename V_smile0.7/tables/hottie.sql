create table hottie
(
	id int unsigned not null auto_increment primary key,
	author char(32) not null,
	picture int not null,
	title char(64) not null,
	text LONGTEXT not null
);
