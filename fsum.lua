--[[
> fsum = require 'fsum'
> = fsum(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1) - 1
0
> fadd, ftotal = fsum()
> for i = 1, 10 do fadd(0.1) end
> = ftotal() - 1
0
> ftotal(0)    -- clear calculator
> for i = 1, 10 do fadd(0.1) end
> = ftotal() - 1
0
--]]

local function fsum(...)
    local p, abs = {1}, math.abs    -- p[1] == #p
    local function fadd(x)
        local p, i = p, 2
        for j = 2, p[1] do
            local y = p[j]
            if abs(x) > abs(y) then x, y = y, x end
            local hi = x + y
            local lo = x - (hi - y)
            x = hi
            if lo ~= 0 then x = lo; p[i] = hi; i = i + 1 end
        end
        if x ~= x then p[1] = 2 return end  -- Inf or NaN
        p[i] = x
        p[1] = i
    end
    local function ftotal(clear)
        if clear then p[1] = 1 end  -- clear all partials
        repeat
            local ok, prev = true, {unpack(p, 1, p[1])}
            fadd(0)                 -- remove partials overlap
            if (p[1] <= 3) then return p[2] end
            for i = 2, p[1] do
                if p[i] ~= prev[i] then ok = false; break end
            end
        until ok
        local x, lo, err = unpack(p, 2, 4)
        if (lo < 0) == (err < 0) then
            lo = lo * 2             -- |lo| = 1/2 ULP
            local hi = x + lo       -- -> x off 1 ULP
            if lo == hi - x then x = hi end
        end
        return x
    end
    if select('#', ...) == 0 then return fadd, ftotal end
    for i = 1, select('#', ...) do fadd(select(i, ...)) end
    return ftotal()
end

if select(1, ...) ~= 'fsum' then    -- test code
    local read, fadd, ftotal = io.read, fsum()
    io.input(select(1, ...))        -- read from file
    pcall(function() while true do fadd(read('*n')) end end)
    print(ftotal())
end
return fsum
