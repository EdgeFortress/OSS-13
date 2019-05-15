local Turf = require "Object.Turf"
derive(Turf, ...)

function New(this)
	Turf.New(this);
	this.layer = 15;
	this.sprite = "floor";
	this.name = "Floor";
	this.density = false;
end
