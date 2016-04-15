function polynomial(t, x)
	sum = 0
	for i in 1, #t do
		sum = sum + t[i] * (x ^ i)
	end
	return sum
end
