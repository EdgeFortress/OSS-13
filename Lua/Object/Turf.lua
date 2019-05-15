local Object = require "Object"
derive(Object, ...)

function New(this)
	Object.New(this);
	this.layer = 25;
	this.movable = false;
end
