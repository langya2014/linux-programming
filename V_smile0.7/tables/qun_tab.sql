create table qun_tab
(
	id int unsigned not null auto_increment primary key,
	qunname char(32) not null,
	qunmaster char(32) not null,
	qunportrait int null,
	qundescrible MEDIUMTEXT null
);
