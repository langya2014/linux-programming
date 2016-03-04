create table user_qun
(
	id int unsigned not null auto_increment primary key,
	username char(32) not null,
	qunname char(32) not null,
	relationship int not null default "1",
	readmsgid int not null default "0"
);
