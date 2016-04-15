function values(t)
	local i = 0
	return function () i = i + 1; return t[i] end
end

function fromto(n, m, s)
    local i = n
    local step = s or 1
    return function ()
        local j = i
        if i < m then
            i = i + step
            return j
        end
    end
end
