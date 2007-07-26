function [path,oldln]=demo_wp_begin(fil)
mode(-1);
if fil~='nofile' then
  path=get_absolute_file_path(fil);
end
oldln=lines();lines(0);
endfunction

