local Projectile = require "Projectile"
local Object = require "Object"
derive(Object, ...)

function New(this)
	Object.New(this);
	this.sprite = "taser";
end
function InteractWith(this, o)
	if not o or not o:GetTile() or not this:GetTile() then return; end;
	Projectile(this:GetTile(), o:GetTile():GetPos() - this:GetTile():GetPos());
end
