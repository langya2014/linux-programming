create table grp_request_msg
(
	id int unsigned not null auto_increment primary key,
	img int not null,
	request char(32) not null,
	qunname char(32) not null,
	qunmaster char(32) not null,
	status char(32) not null default "undo"
);