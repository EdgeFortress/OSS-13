local Object = require "Object"
derive(Object, ...)

function New(this)
	Object.New(this);
	this.sprite = "grey";
	this.slot = 2;
end
