function fact(n)
	if n < 0 then
		return 0
	elseif n == 0 then
		return 1
	else
		return fact(n-1) * n
	end
end

print("Enter a number:")
a = io.read("*n")
print(fact(a))
