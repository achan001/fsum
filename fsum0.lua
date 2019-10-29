--[[
lua> fsum = require 'fsum'
lua> t = 0.1
lua> p = fsum(t,t,t,t,t, t,t,t,t,t)
lua> = p:total() - 1
0

lua> p:add(-1)     -- get error
lua> = p:total()
5.551115123125783e-017

lua> p:total(-1)   -- p = -1
lua> for i = 1, 10 do p:add(0.1) end
lua> = p:total()
5.551115123125783e-017
--]]

local function fadd(p, x)
    local i = 2
    for j = 2, p[1] do              -- p[1] = #p
        local y = p[j]
        local hi = x + y
        local yy = hi - x
        y = y - yy
        y = x - (hi - yy) + y       -- error term
        x = hi
        if y ~= 0 then p[i] = y; i = i + 1 end
    end
    if x - x ~= 0 then i = 2 end    -- Inf / NaN
    p[1] = i
    p[i] = x
end

local function ftotal(p, x)
    if x then p[1]=2; p[2]=x end
    x = 0
    for i = p[1], 2, -1 do          -- sum in reverse
        local y = p[i]
        local hi = x + y
        y = y - (hi - x)
        x = hi
        if y ~= 0 and i ~= 2 then   -- check half way cases
            y = y + y
            if y ~= y+x-x then return x end -- |y| < 1 ULP
            return (y<0) == (p[i-1]<0) and x+y or x
        end
    end
    return x
end

local function fsum(...)
    local n = select('#', ...)
    local p = {add = fadd, total = ftotal, 1}
    for i = 1, n do p:add(select(i, ...)) end
    return p
end

if select(1, ...) ~= 'fsum0' then   -- test code
    local p, read = fsum(), io.read
    io.input(select(1, ...))        -- read from file
    pcall(function() while true do fadd(p, read('*n')) end end)
    print(p:total())
end
return fsum
