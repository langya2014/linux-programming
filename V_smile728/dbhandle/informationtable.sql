create table users
(
	id int unsigned not null auto_increment primary key,
	username char(16) not null,
	sex char(8) not null,
	age smallint unsigned not null,
	city char(12) not null,
	usertype char(12) not null,
	facetype char(12) not null,
	tel char(13) not null,
	mail char(24) null default "-",
	password char(24) not null,
	signature char(32) null default "-",
	portrait longblob
);
