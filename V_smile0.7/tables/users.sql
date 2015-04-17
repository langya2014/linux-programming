create table users
(
	id int unsigned not null auto_increment primary key,
	username char(32) not null,
	sex int null,
	age int unsigned null,
	facetype smallint null,
	mail char(24) null default "-",
	passwd char(24) not null,
	portrait int null,
	socket int not null default "-1"
);
