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
    local n = p[1]
    if n <= 2 then return n<2 and 0 or p[2] end
    local x = p[2] + p[3]
    if n == 3 then return x end
    local y = p[3] - (x - p[2]) -- x + y = p[2] + p[3]
    local z = p[4]
    if (y < 0) == (z < 0) and z ~= 0 then
        y = y * 2               -- if |y| = 1/2 ULP
        z = x + y               -- then |y+z| > 1/2 ULP
        if y == z - x then return z end
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
