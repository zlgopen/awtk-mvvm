ValueValidator('water_temp', {
    isValid: function (v) {
        if (v <= 20) {
            return { result: false, message: "too low" };
        } else if (v >= 60) {
            return { result: false, message: "too high" };
        } else {
            return { result: true, message: "normal" };
        }
    }
});
