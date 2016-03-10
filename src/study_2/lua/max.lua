function maximum(a)
	local mi = 1
	local m = a[mi]
	for i = 1, #a do
		if a[i] > m then
			mi = i;
			m = a[i]
		end
	end

	return m, mi
end
