for i = 1, 9 do
	s = ""
	for j = 1, i do
		s = s .. j .. "x" .. i .. "=" .. (i*j) .. " "
	end
	print(s)
end
