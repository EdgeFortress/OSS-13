print "Loading Lua"

package.path = "Lua/?.lua"

function _new(base, t)
	if type(t) == 'userdata' then
		t = {_instance=t}
		t._instance.instance = t
	else
		t = t or {}
		t._instance = _object(t)
	end
	t._prototype = base
	setmetatable(t,_metaobj)
	t:New()
	return t
end

_metaobj = {
	__index = function(this, key)
		if this._instance[key] ~= nil then
			return this._instance[key]
		end
		return this._prototype[key]
	end,
	__newindex = function(this, key, value)
		if this._instance[key] ~= nil then
			if(type(this._instance[key]) ~= type(value)) then
				error("attempt to assign "..type(value).." ("..value..") to "..type(this._instance[key]).." field", 2)
			end
			this._instance[key] = value
		else
			rawset(this, key, value)
		end
	end
}
_metaclass = {
	__call = _new,
	__index = function(this, key)
		return this._prototype[key]
	end
}

package.loaded["Projectile"] = function(t, v) return _projectile(t, v) end

local function _class(...)
	local M = {_G=_G}
	print ("Loading class ".. ...)
	package.loaded[...] = M
	package.loaded[string.match(..., "[^.]*$")] = M
	setfenv(3,M)
	return M
end

function base(m, ...)
	local M = _class(...)
	setmetatable(M,{
		__call = _new
	})
	for k,v in pairs(m) do
		if not k:match("^__") and type(v) == 'function' then
			print ("Bound ".. ... ..":"..k)
			M[k] = function (this, ...)
				return this._instance[k](this._instance, ...)
			end
		end
	end
end

function derive(m, ...)
	local M = _class(...)
	M._prototype = m
	M._type = string.match(..., "[^.]*$")
	setmetatable(M,_metaclass)
end

require "Object"
require "Object.Taser"
require "Object.Uniform"
require "Object.Turf"
require "Object.Turf.Floor"
require "Object.Turf.Wall"

print "Loaded Lua"
