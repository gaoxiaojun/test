function new_poly(t)
	return function (x)
		local sum = 0
		for i = 1, #t do
			sum = sum + t[i] * x ^ (#t-i)
		end
		return sum
	end
end
