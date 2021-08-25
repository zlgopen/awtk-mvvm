ViewModel('temperature_ex', {
  data: {
    value: 20,
    applydValue: 20
  },
  methods: {
    canApply: function(args) {
      return this.applydValue != this.value;
    },
    apply: function(args) {
      this.applydValue = this.value;
      this.notifyPropsChanged();
    }
  },
  onWillMount: function(req) {
    this.value = req.value || 20;
  }
});
