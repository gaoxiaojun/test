function read_file(file)
    for line in io.lines(file) do
        print(line)
    end
end

local function getnext(list, node)
    if not node then
        return list
    else
        return node.next
    end
end

function traverse(list)
    return getnext, list, nil
end

list = nil
for line in io.lines("readfile.lua") do
    list = {var=line, next=list}
end

for node in traverse(list) do
    print(node.var)
end
