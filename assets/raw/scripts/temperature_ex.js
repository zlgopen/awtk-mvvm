// Model
function Temperature (req) {
  this.value = req.value || 20;
  this.saved_value = 20;
}

Temperature.prototype.save = function(args) {
  this.saved_value = this.value;

  this.notifyPropsChanged();
  return true;
}

Temperature.prototype.canSave = function(args) {
  return this.saved_value != this.value;
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
ValueValidators.water_temp = {
  isValid: function(v) {
    if (v <= 20) {
      return {result: false, message:"two low"};
    } else if (v >= 60) {
      return {result: false, message:"two high"};
    } else {
      return {result: true, message:"normal"};
    }
  }
}
