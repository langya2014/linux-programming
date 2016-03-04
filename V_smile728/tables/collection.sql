create table collection
(
	id int unsigned not null auto_increment primary key,
	collector char(32) not null,
	author char(32) not null,
	title char(64) not null,
	img int not null
);
