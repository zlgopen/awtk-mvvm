var temperature_ex = {
  temp : 20,
  saved_temp : 20,
  save: function(v) {
    this.saved_temp = this.temp;

    this.notifyPropsChanged();
    return true;
  },
  canSave: function(v) {
    return this.saved_temp != this.temp;
  }
}

ValueConverters.fahrenheit = {
  toView: function(v) {
    return v * 1.8 + 32;
  },
  toModel: function(v) {
    return (v - 32) / 1.8;
  }
}

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
