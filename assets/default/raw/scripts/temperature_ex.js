var helper = require('temperature_helper')

// Model
function Temperature (req) {
  this.value = req.value || 20;
  this.applydValue = 20;
}

Temperature.prototype.apply = function(args) {
  this.applydValue = this.value;

  return RET_OBJECT_CHANGED;
}

Temperature.prototype.canApply = function(args) {
  return this.applydValue != this.value;
}

// Model creator
function createTemperatureEx(req) {
  return new Temperature(req);
}

// ValueConverters
ValueConverters.fahrenheit = helper.fahrenheit;

// ValueValidators
ValueValidators.waterTemp = helper.waterTemp;
