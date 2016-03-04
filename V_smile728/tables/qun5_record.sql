create table qun5_record
(
	id int unsigned not null auto_increment primary key,
	qunname char(32) not null,
	username char(32) not null,
	cur_time char(64) not null,
	arecord MEDIUMTEXT not null
);