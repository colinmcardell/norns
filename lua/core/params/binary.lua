--- Binary (toggling) parameters
--
-- See also the [norns script reference](https://monome.org/docs/norns/reference/)
-- which has
-- [examples for using params](https://monome.org/docs/norns/reference/params).
--
-- @module params.binary

local Binary = {}
Binary.__index = Binary

local tBINARY = 9

function Binary.new(id, name, behavior, default, allow_pmap)
  local o = setmetatable({}, Binary)
  o.t = tBINARY
  o.id = id
  o.name = name
  o.default = default or (o.behavior ~= 'trigger' and 0 or 1)
  o.value = o.default
  o.behavior = behavior or 'trigger'
  o.action = function() end
  if allow_pmap == nil then o.allow_pmap = true else o.allow_pmap = allow_pmap end
  return o
end

function Binary:get()
  return self.value
end

function Binary:set(v, silent)
  local silent = silent or false
  local i = params.lookup[self.id]
  v = v and v or 1
  if self.behavior == 'trigger' and v > 0 then
    _menu.binarystates.triggered[i] = 2
    if silent == false then self:bang() end
  elseif self.behavior ~= 'trigger' then
    v = (v > 0) and 1 or 0
    if self.value ~= v then
      self.value = v
      _menu.binarystates.on[i] = v
      if silent == false then self:bang() end
    end
  end
end

function Binary:delta(d)
  if self.behavior == 'momentary' then
    self:set(d)
  elseif self.behavior == 'toggle' then
    if d ~= 0 then self:set((self.value == 0) and 1 or 0) end
  elseif d ~= 0 then
    self:bang()
  end
end

function Binary:set_default()
  self:set(self.default)
end

function Binary:bang()
  self.action(self.behavior == 'trigger' and 1 or self.value)
  if self.behavior == 'trigger' then
    self.value = 0
  end
end

function Binary:string()
  return self.value
end

function Binary:get_range()
  return { 0, 1 }
end

return Binary
