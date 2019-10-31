--[[
lua> fsum = require 'fsum'
lua> t = 0.1
lua> p = fsum(t,t,t,t,t, t,t,t,t,t)
lua> = p:total() - 1
0
lua> p:add(-1)     -- get error
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
        x = x - (hi - yy) + y       -- error term
        if x == 0 then x = hi else p[i] = hi; i = i + 1 end
    end
    if x ~= x then p[1] = 2 return end
    p[1] = i
    p[i] = x
end

local function ffma(p, a,b)         -- p += a*b
    local ha = a * (2^27+1)
    local hb = b * (2^27+1)
    local x = a * b
    ha = a + ha - ha    -- hi bits
    hb = b + hb - hb
    a = a - ha          -- lo bits
    b = b - hb
    p:add(x)
    p:add(a*b - (x - ha*hb - ha*b - hb*a))
end

local function ftotal(p, x, y)      -- y = local variable
    if x then p[1]=2; p[2]=x end    -- p = x
    while true do
        local n = p[1]
        if n <= 2 then return n<2 and 0 or p[2] end
        if n == 3 then return p[2] + p[3] end
        x = p[n]    -- partials overlap ?
        repeat n=n-1; y=0.5*x; x=p[n] until n <= 2 or x ~= x+y
        if x == x+y then break else p:add(0) end
    end
    y = y + y       -- x + 2y => r + y/2
    local r = x + y
    y = 2*(y - (r - x))
    if y ~= y+r-r then return r end -- 0 < |y| < 1 ULP
    return (y<0) == (p[4]<0) and p[4] ~= 0 and r+y or r
end

local function fsum(...)
    local n = select('#', ...)
    local p = {add=fadd, fma=ffma, total=ftotal, 1}
    for i = 1, n do p:add(select(i, ...)) end
    return p
end

if select(1, ...) == 'fsum' then return fsum end

local p, read = fsum(), io.read     -- test code
io.input(select(1, ...))
pcall(function() while true do fadd(p, read('*n')) end end)
print(p:total())
