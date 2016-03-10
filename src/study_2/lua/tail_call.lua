function tail_call(n)
	if n > 0 then
		print(n)
		return tail_call(n-1)
	end
end
