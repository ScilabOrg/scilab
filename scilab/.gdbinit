set breakpoint pending on

define scilabbreak
  if $argc == 0
    break types::DynamicFunction::call
  end
  if $argc == 1
    break types::DynamicFunction::call if wide_string_UTF8_cmp(this->m_wstName, "$arg0") == 0
  end
end

document scilabbreak
  Break on a scilab function / macros / gateway. An optionnal name argument can be provided to only break on a scilab symbol name.
end

