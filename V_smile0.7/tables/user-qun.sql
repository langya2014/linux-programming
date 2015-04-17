create table user_qun
(
	id int unsigned not null auto_increment primary key,
	username char(32) not null,
	qunname char(32) not null,
	msgnotice smallint not null,
	exitqun smallint not null default "0"
);
