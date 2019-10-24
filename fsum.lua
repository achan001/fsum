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

local abs = math.abs
local function fadd(p, x)
    local i = 2
    for j = 2, p[1] do              -- p[1] = #p
        local y = p[j]
        if abs(x) > abs(y) then x,y = y,x end
        local hi = x + y
        y = hi - y
        x = x - y                   -- error term
        if x == 0 then x = hi else p[i] = hi; i = i + 1 end
    end
    if x ~= x then p[1] = 2 return end
    p[1] = i
    p[i] = x
end

local function ftotal(p, x)
    if x then p[1]=2; p[2]=x end
    while true do
        local i, n = 2, p[1]
        if n <= 2 then return n<2 and 0 or p[2] end
        if n == 3 then return p[2] + p[3] end
        while i<n and p[i] == p[i] + p[i+1] do i=i+1 end
        if i == n then break end    -- partials converged
        p:add(0)
    end
    x = p[2]
    local u = 2*p[3]                -- |u| = 1 ULP ?
    if u ~= u+x-x then return x end -- not half-way
    return (u<0) == (p[4]<0) and p[4] ~= 0 and x+u or x
end

local function fsum(...)
    local n = select('#', ...)
    local p = {add = fadd, total = ftotal, 1}
    for i = 1, n do p:add(select(i, ...)) end
    return p
end

if select(1, ...) ~= 'fsum' then    -- test code
    local p, read = fsum(), io.read
    io.input(select(1, ...))        -- read from file
    pcall(function() while true do fadd(p, read('*n')) end end)
    print(p:total())
end
return fsum
