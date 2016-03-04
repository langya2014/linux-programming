create table class_tab
(
	id int unsigned not null auto_increment primary key,
	classname varchar(64) not null,
	roll_call int not null,
	attendace int not null
);
