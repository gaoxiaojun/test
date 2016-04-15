function cat(...)
	local t = {...}
	local s = ""
	for k, v in ipairs(t) do
		s = s .. v
	end
	return s
end

function my_print(arg)
	print(table.unpack(arg))
end

function ret_exp_1(...)
	local arg = table.pack(...)
	return table.unpack(arg, 2, #arg)
end

function cnm(t, n, m)
	if n <= m then
		return t
	elseif m == 0 then
		return {}
	else
		return nil
	end
end
