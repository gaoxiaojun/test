mt = {}
for i = 1, 6 do
    mt[i] = {}
    for j = 1, i do
        mt[i][j] = i * j
    end
end

for i, t in ipairs(mt) do
    for k, v in pairs(t) do
        --print(k, v)
        io.write(v .. " ")
    end
    io.write("\n")
end
