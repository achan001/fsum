--[[
> fsum = require 'fsum0'
> = fsum(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1) - 1
0
> p = fsum()
> for i = 1, 10 do p:add(0.1) end
> = p:total() - 1
0
> p:total(0)    -- clear calculator
> for i = 1, 10 do p:add(0.1) end
> = p:total() - 1
0
--]]
local abs = math.abs
local function fadd(p, x)
    local i = 2
    for j = 2, p[1] do              -- p[1] = #p
        local y = p[j]
        if abs(x) < abs(y) then x, y = y, x end
        local hi = x + y
        local lo = y - (hi - x)
        x = hi
        if lo ~= 0 then p[i] = lo; i = i + 1 end
    end
    if x - x ~= 0 then i = 2 end    -- Inf or NaN
    p[i] = x
    p[1] = i
end
local function ftotal(p, clear)
    if clear then p[1] = 1 end      -- clear all partials
    local x = 0
    for i = p[1], 2, -1 do          -- sum in reverse
        local y = p[i]
        local hi = x + y
        local lo = y - (hi - x)
        x = hi
        if lo ~= 0 and i ~= 2 then  -- check half way cases
            if (lo < 0) == (p[i-1] < 0) then
                lo = lo * 2         -- |lo| = 1/2 ULP
                hi = x + lo         -- -> x off 1 ULP
                if lo == hi - x then x = hi end
            end
            break
        end
    end
    return x
end
local function fsum(...)
    local p = {add = fadd, total = ftotal, 1}
    if select('#', ...) == 0 then return p end
    for i = 1, select('#', ...) do p:add(select(i, ...)) end
    return p:total()
end

if select(1, ...) ~= 'fsum0' then   -- test code
    local p, read = fsum(), io.read
    io.input(select(1, ...))        -- read from file
    pcall(function() while true do fadd(p, read('*n')) end end)
    print(p:total())
end
return fsum
