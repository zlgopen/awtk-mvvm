/*demo module*/

exports.fahrenheit = {
  toView: function(v) {
    return v * 1.8 + 32;
  },
  toModel: function(v) {
    return (v - 32) / 1.8;
  }
}

exports.waterTemp = {
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
