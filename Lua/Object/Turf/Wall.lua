local Turf = require "Object.Turf"
derive(Turf, ...)

function New(this)
	Turf.New(this);
	this.sprite = "wall";
	this.name = "Wall";
	this.density = true;
end
