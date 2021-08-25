ValueConverter('fahrenheit', {
    toView: function (v) {
        return v * 1.8 + 32;
    },
    toModel: function (v) {
        return (v - 32) / 1.8;
    }
});