--[[
lua> fsum = require 'fsum0'
lua> t = 0.1
lua> p = fsum(t,t,t,t,t, t,t,t,t,t)
lua> = p:total() - 1
0
lua> p:add(-1):total()     -- get error
5.551115123125783e-017
--]]

local function fadd(p, x)
    local i, n = 2, p[1]
    for j = 2, n do
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

    if i <= n then return p end
    for i = n, 3, -2 do             -- stack expanded
        x = p[i]
        local y = p[i-1]
        local hi = x + y
        p[i-1] = y - (hi - x)       -- possibly zero
        p[i] = hi
    end
    return p
end

local function ffma(p, a,b)         -- p += a*b
    local ha = a * (2^27+1)
    local hb = b * (2^27+1)
    local x = a * b
    ha = a - ha + ha    -- hi bits
    hb = b - hb + hb
    a = a - ha          -- lo bits
    b = b - hb
    p: add(x) :add(a*b - (x - ha*hb - ha*b - hb*a))
end

local function ftotal(p, x)
    if x then p[1]=2; p[2]=x end
    x = 0
    for i = p[1], 2, -1 do          -- sum in reverse
        local y = p[i]
        local hi = x + y
        y = y - (hi - x)
        x = hi
        if y ~= 0 and i > 2 then    -- check half way cases
            y = y + y
            if y ~= y+x-x then return x end -- |y| < 1 ULP
            local z = i==3 and p[2] or p[i-1] + p[i-2]
            return (y<0) == (z<0) and z ~= 0 and x+y or x
        end
    end
    return x
end

local function fsum(...)
    local n = select('#', ...)
    local p = {add=fadd, fma=ffma, total=ftotal, 1}
    for i = 1, n do p:add(select(i, ...)) end
    return p
end

if package.loading() then return fsum end

local p, read = fsum(), io.read     -- test code
io.input(select(1, ...))
pcall(function() while true do fadd(p, read('*n')) end end)
print(p:total())
