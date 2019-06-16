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

// ValueValidators
ValueConverters.fahrenheit = {
  toView: function(v) {
    return v * 1.8 + 32;
  },
  toModel: function(v) {
    return (v - 32) / 1.8;
  }
}

// ValueValidators
ValueValidators.waterTemp = {
  isValid: function(v) {
    if (v <= 20) {
      return {result: false, message:"too low"};
    } else if (v >= 60) {
      return {result: false, message:"too high"};
    } else {
      return {result: true, message:"normal"};
    }
  }
}
