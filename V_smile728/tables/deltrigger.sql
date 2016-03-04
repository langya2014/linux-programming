delimiter $
create trigger delQunTrigger
     after delete on qun_tab
     for each row
     begin
     delete from qun0_record where qunname=old.qunname;
     delete from user_qun where qunname = old.qunname;
     end

delimiter ;