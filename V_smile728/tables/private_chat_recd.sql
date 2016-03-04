create table private_chat_recd
(
	id int unsigned not null auto_increment primary key,
	speaker char(32) not null,
	listener char(32) not null,
	cur_time TIMESTAMP not null,
	content MEDIUMTEXT not null
);
