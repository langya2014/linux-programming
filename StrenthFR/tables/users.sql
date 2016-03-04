create table users
(
	id int unsigned not null auto_increment primary key,
	username varchar(64) not null,
	sex int null,
	passwd char(32) not null,
	socket int not null default "-1"
);
