--[[
lua> fsum = require 'fsum'
lua> t = 0.1
lua> p = fsum(t,t,t,t,t, t,t,t,t,t)
lua> = p:total()
1

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
        if abs(y) < abs(x) then j=y y=x x=j end
        local hi = x + y
        y = hi - y
        x = x - y                   -- error term
        if x == 0 then x = hi else p[i] = hi; i = i + 1 end
    end
    if x ~= x then p[1] = 2 return end
    p[1] = i
    p[i] = x
end
local function ftotal(p, value)
    if value then p[1]=2; p[2]=value end
    repeat
        local i, n = 1, p[1]
        local prev = {unpack(p, i, n)}
        fadd(p, 0)                  -- remove partials overlap
        if n <= 3 then return p[2] end
        while i <= n and p[i] == prev[i] do i = i + 1 end
    until i > n
    local x, y, err = unpack(p, 2, 4)
    if (y < 0) == (err < 0) then    -- check half-way cases
        y = y * 2                   -- |y| = 1/2 ULP
        local hi = x + y            -- -> x off 1 ULP
        if y == hi - x then x = hi end
    end
    return x
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
